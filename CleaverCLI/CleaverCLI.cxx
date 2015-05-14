#include "CleaverCLICLP.h"
#include <Cleaver/Cleaver.h>
#include <Cleaver/Volume.h>
#include <Cleaver/PaddedVolume.h>
#include <Cleaver/FloatField.h>
#include <Cleaver/InverseField.h>
#include "itkImageFileReader.h"
#include <fstream>

// Use an anonymous namespace to keep class types and function names
// from colliding when module is used as shared object module.  Every
// thing should be in an anonymous namespace except for the module
// entry point, e.g. main()
//

using namespace Cleaver;

namespace
{
  std::vector<ScalarField*> loadNRRDFiles(
      const std::vector<std::string> &filenames,
      bool verbose = false)
  {
    typedef float                                InputPixelType;
    typedef itk::Image<InputPixelType, 3>        InputImageType;
    typedef itk::ImageFileReader<InputImageType> ReaderType;
    //--------------------------------------------
    //  Read in the files
    //-------------------------------------------
    std::vector<InputImageType::Pointer> nins;
    for(unsigned int i=0; i < filenames.size(); i++)
    {
      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(filenames.at(i).c_str() );
      reader->Update();
      InputImageType::Pointer inputImage = reader->GetOutput();
      // Read in the Header
      if(verbose)
        std::cout << "Reading File: " << filenames[i] << std::endl;
      nins.push_back(inputImage);
    }
    //-----------------------------------
    // Verify contents match
    //-----------------------------------
    bool match = true;

    for(unsigned i=1; i < filenames.size(); i++)
    {
      for(unsigned int j=0; j < 3; j++)
        if(nins[i]->GetLargestPossibleRegion().GetSize()[j] !=
            nins[0]->GetLargestPossibleRegion().GetSize()[j])
        {
          std::cerr << "Error, file " << j << " dimensions don't match." << std::endl;
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
    //       Input Dimensions
    //-----------------------------------
    int w = nins[0]->GetLargestPossibleRegion().GetSize()[0];
    int h = nins[0]->GetLargestPossibleRegion().GetSize()[1];
    int d = nins[0]->GetLargestPossibleRegion().GetSize()[2];

    if(verbose)
      std::cout << "Input Dimensions: " << w << " x " << h << " x " << d << std::endl;

    //---------------------------------------
    //     Allocate Sufficient Data
    //---------------------------------------
    std::vector<ScalarField*> fields;

    for(unsigned int f=0; f < filenames.size(); f++)
    {
      float *data = new float[w * h * d];
      fields.push_back(new FloatField(w,h,d, data));
    }

    //----------------------------------------
    //  Deferred  Data Load/Copy
    //----------------------------------------
    for(unsigned int f=0; f < filenames.size(); f++)
    {
      // cast and copy into large array
      for(int k=0; k < d; k++){
        for(int j=0; j < h; j++){
          for(int i=0; i < w; i++){
            itk::Index<3> idx;
            idx.SetElement(0,i);
            idx.SetElement(1,j);
            idx.SetElement(2,k);
            ((FloatField*)fields[f])->data()[i + j*w + k*w*h] =
              static_cast<float>(nins[f]->GetPixel(idx));
          }
        }
      }

      // set scale
      float xs = nins[f]->GetSpacing()[0];
      float ys = nins[f]->GetSpacing()[1];
      float zs = nins[f]->GetSpacing()[2];

      // handle NaN cases
      if(xs != xs) xs = 1;
      if(ys != ys) ys = 1;
      if(zs != zs) zs = 1;

      ((FloatField*)fields[f])->setScale(vec3(xs,ys,zs));
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

  if (scale <= 0) scale = 1.0;
  ((Cleaver::Volume*)volume)->setSize(scale*volume->size().x,
    scale*volume->size().y,
    scale*volume->size().z);

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
  if (!outputDir.empty() && outputDir.at(outputDir.size() - 1) != '/')
    outputDir = outputDir + "/";
  std::string output = outputDir + outputMesh1;
  if (output.empty())
    output = "output";
  std::vector<std::string> outputs;
  if(!outputMesh1.empty())
    outputs.push_back(outputDir + outputMesh1);
  if(!outputMesh2.empty())
    outputs.push_back(outputDir + outputMesh2);
  if(!outputMesh3.empty())
    outputs.push_back(outputDir + outputMesh3);
  if(!outputMesh4.empty())
    outputs.push_back(outputDir + outputMesh4);
  if(!outputMesh5.empty())
    outputs.push_back(outputDir + outputMesh5);
  if(!outputMesh6.empty())
    outputs.push_back(outputDir + outputMesh6);
  if(!outputMesh7.empty())
    outputs.push_back(outputDir + outputMesh7);
  if(!outputMesh8.empty())
    outputs.push_back(outputDir + outputMesh8);
  if(!outputMesh9.empty())
    outputs.push_back(outputDir + outputMesh9);
  if(!outputMesh10.empty())
    outputs.push_back(outputDir + outputMesh10);


  //----------------------
  // Write Surface Files
  //----------------------
  mesh->constructFaces();

  //----------------------
  // Write Tet Mesh Files
  //----------------------
  if(outputFormat == "tetgen") {
    mesh->writeNodeEle(output, verbose);
    mesh->writePly(output, verbose);
    mesh->writeInfo(output, verbose);
  }
  else if(outputFormat == "scirun") {
    mesh->writePtsEle(output, verbose);
    mesh->writePly(output, verbose);
    mesh->writeInfo(output, verbose);
  }
  else if(outputFormat == "matlab") {
    mesh->writeMatlab(output, verbose);
    mesh->writePly(output, verbose);
    mesh->writeInfo(output, verbose);
  }
  else if(outputFormat == "VTKtet")
    mesh->writeVTKunstructuredMeshTets(outputs, verbose);
  else if(outputFormat == "VTKfac")
    mesh->writeVTKunstructuredMesh(outputs, verbose);
  else  {
    std::cerr << "Uknown format: " << outputFormat <<
      ". Using default: VTKtet." << std::endl;
    mesh->writeVTKunstructuredMeshTets(outputs, verbose);
  }

  //std::cout << "MRML->" << mesh->outputMRML() << std::endl;


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
