#include "CleaverCLICLP.h"
#include <Cleaver/Cleaver.h>
#include <Cleaver/Volume.h>
#include <Cleaver/PaddedVolume.h>
#include <Cleaver/FloatField.h>
#include <Cleaver/InverseField.h>
#include <teem/nrrd.h>
#include <fstream>

// Use an anonymous namespace to keep class types and function names
// from colliding when module is used as shared object module.  Every
// thing should be in an anonymous namespace except for the module
// entry point, e.g. main()
//

using namespace Cleaver;

namespace
{
   const float REALLY_SMALL = -10000;
   const float REALLY_LARGE = +10000;


   ScalarField* loadNRRDFile(const std::string &filename, bool verbose)
   {
      //--------------------------------------------
      //  Read header
      //-------------------------------------------

      // create empty nrrd file container
      Nrrd* nin = nrrdNew();

      // load only the header, not the data
      NrrdIoState *nio = nrrdIoStateNew();
      nrrdIoStateSet(nio, nrrdIoStateSkipData, AIR_TRUE);

      // Read in the Header
      if(nrrdLoad(nin, filename.c_str(), nio))
      {
         char *err = biffGetDone(NRRD);
         std::cerr << "Trouble Reading File: " << filename << " : " << err << std::endl;
         free(err);
         nio = nrrdIoStateNix(nio);
         return NULL;
      }

      // Done with nrrdIoState
      nio = nrrdIoStateNix(nio);

      if(nin->dim != 3)
      {
         std::cout << "Fatal Error: volume dimension " << nin->dim << ", expected 3." << std::endl;
         return NULL;
      }

      //-----------------------------------
      //       Account For Padding
      //-----------------------------------
      int w = nin->axis[0].size;
      int h = nin->axis[1].size;
      int d = nin->axis[2].size;

      //---------------------------------------
      //     Allocate Sufficient Data
      //---------------------------------------
      float *data = new float[w * h * d];


      //----------------------------------------
      //  Deferred  Data Load/Copy
      //----------------------------------------
      if(nrrdLoad(nin, filename.c_str(), NULL))
      {
         char *err = biffGetDone(NRRD);
         std::cerr << "trouble reading data in file: " << filename << " : " << err << std::endl;
         free(err);
         return NULL;
      }

      float (*lup)(const void *, size_t I);
      lup = nrrdFLookup[nin->type];

      // cast and copy into float array
      int s=0;
      for(int k=0; k < d; k++){
         for(int j=0; j < h; j++){
            for(int i=0; i < w; i++){
               data[i + j*w + k*w*h] = lup(nin->data, s++);
            }
         }
      }

      // free local copy
      nrrdNuke(nin);
      if(verbose)
         std::cout << "Finished Reading file: " << filename << std::endl;

      //----------------------------------------
      // Create and return ScalarField
      //----------------------------------------
      return (new FloatField(w,h,d,data));
   }

   std::vector<ScalarField*> loadNRRDFiles(const std::vector<std::string> &filenames, bool verbose = false)
   {
      bool pad = false;

      //--------------------------------------------
      //  Read only headers of each file
      //-------------------------------------------
      std::vector<Nrrd*> nins;
      for(unsigned int i=0; i < filenames.size(); i++)
      {
         // create empty nrrd file container
         nins.push_back(nrrdNew());

         // load only the header, not the data
         NrrdIoState *nio = nrrdIoStateNew();
         nrrdIoStateSet(nio, nrrdIoStateSkipData, AIR_TRUE);

         // Read in the Header
         if(verbose)
            std::cout << "Reading File: " << filenames[i] << std::endl;
         if(nrrdLoad(nins[i], filenames[i].c_str(), nio))
         {
            char *err = biffGetDone(NRRD);
            std::cerr << "Trouble Reading File: " << filenames[i] << " : " << err << std::endl;
            free(err);
            nio = nrrdIoStateNix(nio);
            continue;
         }

         // Done with nrrdIoState
         nio = nrrdIoStateNix(nio);

         if(nins[i]->dim != 3)
         {
            std::cerr << "Fatal Error: volume dimension " << nins[i]->dim << ", expected 3." << std::endl;
            std::vector<ScalarField*> empty_vector;
            return empty_vector;
         }
      }

      //-----------------------------------
      // Verify contents match
      //-----------------------------------
      bool match = true;

      for(unsigned i=1; i < filenames.size(); i++)
      {
         if(nins[i]->dim != nins[0]->dim){
            std::cerr << "Error, file " << i << " # dims don't match." << std::endl;
            match = false;
         }

         for(unsigned int j=0; j < nins[0]->dim-1; j++)
            if(nins[i]->axis[j].size != nins[0]->axis[j].size)
            {
               std::cerr << "Error, file " << j << " dimensions don't match." << std::endl;
               match = false;
               break;
            }

         if((int)nrrdElementNumber(nins[i]) != (int)nrrdElementNumber(nins[0]))
         {
            std::cerr << "Error, file " << i << " does not contain expected number of elements" << std::endl;
            std::cerr << "Expected: " << (int)nrrdElementNumber(nins[0]) << std::endl;
            std::cerr << "Found:    " << (int)nrrdElementNumber(nins[i]) << std::endl;

            match = false;
            break;
         }
      }

      if(!match)
      {
         std::vector<ScalarField*> empty_vector;
         return empty_vector;
      }


      //-----------------------------------
      //       Account For Padding
      //-----------------------------------
      int w = nins[0]->axis[0].size;
      int h = nins[0]->axis[1].size;
      int d = nins[0]->axis[2].size;
      int m = filenames.size();
      int p = 0;

      if(verbose)
         std::cout << "Input Dimensions: " << w << " x " << h << " x " << d << std::endl;

      if(pad){
         p = 2;

         // correct for new grid size
         w += 2*p;
         h += 2*p;
         d += 2*p;

         // add material for 'outside'
         m++;

         if(verbose)
            std::cout << "Padded to: " << w << " x " << h << " x " << d << std::endl;
      }


      //---------------------------------------
      //     Allocate Sufficient Data
      //---------------------------------------
      std::vector<ScalarField*> fields;

      for(unsigned int f=0; f < filenames.size(); f++)
      {
         float *data = new float[w * h * d];
         fields.push_back(new FloatField(w,h,d, data));
      }

      if(pad)
      {
         float *data = new float[w * h * d];
         fields.push_back(new FloatField(w,h,d, data));
      }


      //----------------------------------------
      //  Deferred  Data Load/Copy
      //----------------------------------------
      for(unsigned int f=0; f < filenames.size(); f++)
      {
         // load nrrd data, reading memory into data array
         if(nrrdLoad(nins[f], filenames[f].c_str(), NULL))
         {
            char *err = biffGetDone(NRRD);
            std::cerr << "trouble reading data in file: " << filenames[f] << " : " << err << std::endl;
            free(err);
            std::vector<ScalarField*> empty_vector;
            return empty_vector;
         }

         float (*lup)(const void *, size_t I);
         lup = nrrdFLookup[nins[f]->type];

         // cast and copy into large array
         int s=0;
         for(int k=p; k < d-p; k++){
            for(int j=p; j < h-p; j++){
               for(int i=p; i < w-p; i++){
                  ((FloatField*)fields[f])->data()[i + j*w + k*w*h] = lup(nins[f]->data, s++);
               }
            }
         }

         // set scale
         float xs = ((Nrrd*)nins[f])->axis[0].spacing;
         float ys = ((Nrrd*)nins[f])->axis[1].spacing;
         float zs = ((Nrrd*)nins[f])->axis[2].spacing;

         // handle NaN cases
         if(xs != xs) xs = 1;
         if(ys != ys) ys = 1;
         if(zs != zs) zs = 1;

         ((FloatField*)fields[f])->setScale(vec3(xs,ys,zs));

         //if(verbose)
         //  std::cout << "Spacings: " << xs << ", " << ys << ", " << zs << std::endl;


         // free local copy
         nrrdNuke(nins[f]);
      }

      //----------------------------------------------
      //      Copy Boundary Values If Necessary
      //----------------------------------------------
      if(pad)
      {
         // set 'outside' material small everywhere except exterior
         for(int k=0; k < d; k++){
            for(int j=0; j < h; j++){
               for(int i=0; i < w; i++){

                  // is distance to edge less than padding
                  if(k < p || k >= (d - p) ||
                        j < p || j >= (h - p) ||
                        i < p || i >= (w - p))
                  {
                     // set all other materials really small
                     for(unsigned int f=0; f < filenames.size(); f++)
                        ((FloatField*)fields[f])->data()[i + j*w + k*w*h] = REALLY_SMALL;

                     // set 'outside' material really large
                     ((FloatField*)fields[m-1])->data()[i + j*w + k*w*h] = REALLY_LARGE;
                  }
                  // otherwise
                  else{
                     // set 'outside' material very small
                     ((FloatField*)fields[m-1])->data()[i + j*w + k*w*h] = REALLY_SMALL;
                  }
               }
            }
         }
      }

      return fields;
   }



} // end of anonymous namespace

int main( int argc, char * argv[] )
{
   PARSE_ARGS;

   std::vector<std::string> inputs;

   if(!input1.empty())
     inputs.push_back(input1);
   if(!input2.empty())
     inputs.push_back(input2);
   if(!input3.empty())
     inputs.push_back(input3);
   if(!input4.empty())
     inputs.push_back(input4);
   if(!input5.empty())
     inputs.push_back(input5);
   if(!input6.empty())
     inputs.push_back(input6);
   if(!input7.empty())
     inputs.push_back(input7);
   if(!input8.empty())
     inputs.push_back(input8);

   std::vector<Cleaver::ScalarField*> fields = loadNRRDFiles(inputs, verbose);

   if(fields.empty())
      return EXIT_FAILURE;
   else if(fields.size() == 1)
      fields.push_back(new Cleaver::InverseField(fields[0]));

   Cleaver::AbstractVolume *volume = new Cleaver::Volume(fields);

   if(paddingOn)
      volume = new Cleaver::PaddedVolume(volume);

   std::cout << "Creating Mesh with Volume Size " << volume->size().toString() << std::endl;

   //--------------------------------
   //  Create Mesher & TetMesh
   //--------------------------------
   Cleaver::TetMesh *mesh = Cleaver::createMeshFromVolume(volume, verbose);

   //------------------
   //  Compute Angles
   //------------------
   mesh->computeAngles();
   if(verbose){
      std::cout.precision(12);
      std::cout << "Worst Angles:" << std::endl;
      std::cout << "min: " << mesh->min_angle << std::endl;
      std::cout << "max: " << mesh->max_angle << std::endl;
   }

   //----------------------
   //  Write Info File
   //----------------------
//   std::string path = input1.substr(0,input1.rfind("/")+1);
   std::string output = "" + outputMesh;
//   mesh->writeInfo(output, verbose);


   //----------------------
   // Write Surface Files
   //----------------------
   mesh->constructFaces();
//   mesh->writePly(output, verbose);

   //----------------------
   // Write Tet Mesh Files
   //----------------------
   if(outputFormat == "tetgen")
      mesh->writeNodeEle(output, verbose);
   else if(outputFormat == "scirun")
      mesh->writePtsEle(output, verbose);
   else if(outputFormat == "matlab")
      mesh->writeMatlab(output, verbose);
   else if(outputFormat == "VTKusm")
      mesh->writeVTKunstructuredMesh(output, verbose);
   else  {
      std::cerr << "Uknown format: " << outputFormat <<
         ". Using default: tetgen." << std::endl;
      mesh->writeNodeEle(output, verbose);
   }


   //-----------
   // Cleanup
   //-----------
   if(verbose)
      std::cout << "Cleaning up." << std::endl;
   delete mesh;
   for(unsigned int f=0; f < fields.size(); f++)
      delete fields[f];
   delete volume;

   //-----------
   //  Done
   //-----------
   if(verbose)
      std::cout << "Done." << std::endl;

   return EXIT_SUCCESS;
}
