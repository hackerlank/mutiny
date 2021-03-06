#include "Environment.h"
#include "Util.h"
#include "cwrapper.h"
#include "features.h"
#include "FileInfo.h"

#include <iostream>

shared<Environment> Environment::create(std::vector<std::string>& args)
{
  shared<Environment> rtn = alloc_shared<Environment>();
  rtn->args = args;

#ifdef HAS_WINAPI
  std::string absolutePath = Module::getModuleFileName(shared<Module>());
  std::string absoluteBinPath = FileInfo::getFolderPath(absolutePath);
  rtn->prefix = FileInfo::getFolderPath(absoluteBinPath);
#else
  std::string absolutePath = Util::stripEol(Util::execute("which \"" + args.at(0) + "\""));
  std::string absoluteBinPath = Util::stripEol(Util::execute("dirname \"" + absolutePath + "\""));
  rtn->prefix = Util::stripEol(Util::execute("dirname \"" + absoluteBinPath + "\""));
#endif

  rtn->projectName = FileInfo::getFileName(Dir::getcwd());

  //std::cout << "Mutiny Prefix: " << rtn->prefix << std::endl;

  try
  {
    shared<Dir> dir = Dir::opendir(rtn->prefix + "/src/mutiny");
    shared<Dirent> dirent = dir->readdir();

    if(dirent.get() != NULL)
    {
      std::cout << "Mutiny is available" << std::endl;
      rtn->mutinyAvailable = true;
    }

  }
  catch(std::exception& e) { }

#ifdef INTERNAL_GCC
  char *origPath = getenv("PATH");
  std::string origPathStr;

  if(origPath != NULL)
  {
    origPathStr += origPath;
  }

  if(origPathStr.length() > 0)
  {
    origPathStr += ";";
  }

  origPathStr += rtn->prefix + "\\bootstrap\\windows\\mingw32\\bin";

  SetEnvironmentVariable("PATH", origPathStr.c_str());
#endif

  rtn->compilerName = DEFAULT_CXX;

  for(int i = 0; i < args.size(); i++)
  {
    if(args.at(i) == "-o")
    {
      i++;
      rtn->outputDirectory = args.at(i);
    }
    else if(args.at(i) == "-c")
    {
      i++;
      rtn->compilerName = args.at(i);
    }
    else if(args.at(i) == "-s")
    {
      rtn->mutinyAvailable = false;
    }
    else if(args.at(i) == "-g")
    {
      rtn->debug = true;
    }
    else if(args.at(i) == "-d")
    {
      i++;
      rtn->defines.push_back(args.at(i));
    }
  }

  return rtn;
}

std::string Environment::getProjectName()
{
  return projectName;
}

std::vector<std::string> Environment::getDefines()
{
  return defines;
}

std::string Environment::getCompilerName()
{
  return compilerName;
}

std::string Environment::getOutputDirectory()
{
  return outputDirectory;
}

bool Environment::isMutinyAvailable()
{
  return mutinyAvailable;
}

bool Environment::isDebug()
{
  return debug;
}

std::string Environment::getPrefix()
{
  return prefix;
}
