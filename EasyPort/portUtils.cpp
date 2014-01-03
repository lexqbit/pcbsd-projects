#include "portUtils.h"

// ===============
//    INFORMATION
// ===============
QStringList PortUtils::findPortCategories(QString portdir){
  //search through the available ports tree and find the valid categories
  QStringList cats;
  QDir dir(portdir);
  if(dir.exists()){
    QStringList raw = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    for(int i=0; i<raw.length(); i++){
      //check each raw directory to see if it is a valid ports collection
      if( (raw[i]!="distfiles") && raw[i].at(0).isLower() ){
      	cats << raw[i];      
      }
    }
  }
  return cats;
}

QStringList PortUtils::getMakefileConfigOpts(){
  //Output format: <variable>:::<format>:::<description>
  /*   <format>: 
	text - single line/word of text
	textlist - list of distinct text
	port - FreeBSD port category/name
	portbin - binary from a FreeBSD port
	portlib - library from a FreeBSD port
	cats - FreeBSD category list
	url - Some kind of valid URL(s)
  */
	
  QStringList opt;
  //These should be listed in their order of appearance in the Makefile
  opt << "PORTNAME:::text:::"+QObject::tr("Name for the port");
  opt << "PORTVERSION:::text:::"+QObject::tr("Current version of the application");
  opt << "CATEGORIES:::cats:::"+QObject::tr("FreeBSD categories to put the port in");
  opt << "MASTER_SITES:::url:::"+QObject::tr("URL for the master location(s) to look for the application source");
  opt << "MASTER_SITE_SUBDIR:::text:::"+QObject::tr("Master Site subdirectory to look for application source");
  opt << "PKGNAMEPREFIX:::text:::"+QObject::tr("Prefix for the application source file");
  opt << "PKGNAMESUFFIX:::text:::"+QObject::tr("Suffix for the application source file");
  opt << "DISTNAME:::text:::"+QObject::tr("Format/name of the application source file");
  opt << "EXTRACT_SUFX:::text:::"+QString(QObject::tr("Alternate source suffix if not the standard %1")).arg("\"*.tar.gz\"");
  opt << "DISTFILES:::textlist:::"+QObject::tr("Name of the distribution source file(s) to use");
  opt << "EXTRACT_ONLY:::textlist:::"+QObject::tr("");
  opt << "PATCH_SITES:::url:::"+QObject::tr("URL to fetch patch files");
  opt << "PATCHFILES:::textlist:::"+QObject::tr("Names of the patch files to use");
  opt << "MAINTAINER:::text:::"+QObject::tr("Port maintainer email address (usually yours if you are creating it)");
  opt << "COMMENT:::text:::"+QObject::tr("Short description of the application");
  opt << "RUN_DEPENDS:::portbin:::"+QObject::tr("Runtime dependencies for the application");
  
  return opt;
}

QStringList PortUtils::getMakefileOptHints(QString opt){
  //Output format: <user text shown>:::<config text added>
  QStringList hints;
  if(opt == "MASTER_SITES"){
    hints << "Apache Jakarta:::APACHE_JAKARTA";
    hints << "BerliOS:::BERLIOS";
    hints << "CheeseShop:::CHEESESHOP";
    hints << "Debian:::DEBIAN";
    hints << "GCC Project:::GCC";
    hints << "GNOME Project:::GNOME";
    hints << "GNU Project:::GNU";
    hints << "Mozilla Project:::MOZDEV";
    hints << "Perl (CPAN):::PERL_CPAN";
    hints << "Python Project:::PYTHON";
    hints << "RubyForge:::RUBYFORGE";
    hints << "Savannah:::SAVANNAH";
    hints << "SourceForge:::SF";
  }else if(opt == "DISTFILES"){
    hints << "PortName:::${PORTNAME}";
    hints << "PortVersion:::${PORTVERSION}";
  }
  return hints;
}

// ================
//    CONFIGURATIONS
// ================
QStringList PortUtils::generateNewMakefile(QString name, QStringList vars, QStringList vals){
  //QuickPorting requires: PORTNAME (as "name" input), PORTVERSION, CATEGORIES, MASTER_SITES, MAINTAINER, COMMENT
  if(name.isEmpty() || (vars.length() != vals.length()) ){ return QStringList(); } //invalid inputs
  //Get the ordering of the config options
  QStringList opts = PortUtils::getMakefileConfigOpts();
  //Now generate the template
  QStringList out;
  out << "# Port template generated by EasyPort version 1.0";
  out << "# $FreeBSD$";
  out << "";
  out << "# General Port information #";
  out << "PORTNAME=\t"+name;
  //Place the input options in the proper order
  for(int i=0; i<opts.length(); i++){
    int index = vars.indexOf(opts[i].section(":::",0,0));
    if( index >= 0 ){
	out << vars[index]+"=\t"+vals[index];
    }
  }
  out << "";
  out << ".include <bsd.port.mk>";

  return out;
}

QStringList PortUtils::generatePkgDescr(QString description, QString website, QString author){
  QStringList contents;
  contents << description;
  contents << "";
  if(!website.isEmpty()){ contents << "WWW: "+website; }
  if(!author.isEmpty()){ contents << "AUTHOR: "+author; }
  return contents;
}

QStringList PortUtils::insertMakeFileConfig(QStringList current, QString var, QString val, bool replace){
  //Get the valid order of the options in the Makefile
  QStringList opts = PortUtils::getMakefileConfigOpts();
  int CI= -1;
  for(int i=0; i<opts.length(); i++){
    opts[i] = opts[i].section(":::",0,0); //trim off the extra info
    if(opts[i] == var ){ CI = i; }
  }
  if(CI < 0){ return current; } //Invalid makefile option - return the current file contents
  //Now go through the file contents and insert the option in the appropriate spot
  for(int i=0; i<current.length(); i++){
    if(current[i].isEmpty() || current[i].startsWith("#") ){ continue; } //skip this line
    else if(current[i].contains("=")){
      QString cvar = current[i].section("=",0,0).remove("?").simplified();
      int TI = opts.indexOf(cvar); //this index
      if(TI < CI){ 
	continue; //not far enough in the file yet
      }else if(TI == CI){ //This config already exists
	if(replace){
	  //Overwrite the current line and remove any "additional" lines for the same variable
	  int dI = i;
	  while(current[dI].endsWith("\\")){ dI++; }
	  while(dI > i){
	    current.removeAt(dI);
	    dI--;
	  }
	  current[i] = var+"=\t"+val;
	}else{
	  //Just append the value to the current item
	  int II = i;
	  while(current[II].endsWith("\\")){ II++; } //move to the appropriate line
	  if( II > i || var.contains("_DEPENDS") || current[II].length() > 60){ //put it on the next line
	    current.insert(II+1, val);
	    current[II].append(" \\");
	  }else{  //just append it to the current line
	    current[II].append(" "+val);
	  }
	}
	break; //done
      }else{ //TI > CI
	//insert the new config option right before this line
	current.insert(i, var+"=\t"+val);
	break; //done
      }
    }else if( ( current[i].contains(":") && (current[i].startsWith("pre-") || current[i].startsWith("post-") || current[i].startsWith("do-")) ) || current[i].contains(".include <") ){
      //Ran out of config section in the file - just add it to the end of the config section
      current.insert(i, var+"=\t"+val);
      current.insert(i+1,"");
    }
  }
  return current;
}

// ======================
//   GENERAL UTILITIES
// ======================
bool PortUtils::runCmd(QString indir, QString cmd, QStringList args){
  QProcess proc;
  if(!indir.isEmpty() && QFile::exists(indir) ){
    proc.setWorkingDirectory(indir);
  }
  if(args.isEmpty()){ proc.start(cmd); }
  else{ proc.start(cmd,args); }
  while( !proc.waitForFinished(300) ){
    QCoreApplication::processEvents(); //make sure we don't hang the GUI	  
  }
  return (proc.exitCode() == 0);
}

void PortUtils::compressDir(QString dirPath){
  //Compress the directory
  QString localDir = dirPath.section("/",-1);
  //QString cmd = "cd "+dirPath+"/..; tar czf "+localDir+".tar.gz "+localDir;
  QStringList args; args << "czf" << localDir+".tar.gz" << localDir;
  qDebug() << "Compressing dir:" << dirPath+".tar.gz";
  dirPath.chop(localDir.length()); //go up one directory
  PortUtils::runCmd(dirPath, "tar", args);
  return;
}

bool PortUtils::createFile(QString fileName, QStringList contents){
//fileName = full path to file (I.E. /home/pcbsd/junk/junk.txt)
//contents = list of lines to be written (one line per entry in the list - no newline needed at the end of an entry)
	
  //Check if this is actually a file removal
  if(contents.isEmpty() && QFile::exists(fileName)){
    bool good = QFile::remove(fileName);
    if(!good){
      qDebug() << fileName+": Could not be deleted";
    }
    return good;
  }else if(contents.isEmpty()){
    return true; //File to delete already does not exist 
  }
  
  //Check that the parent directory exists, and create it if necessary
  QString pDir = fileName;
  pDir.chop( fileName.section("/",-1).length() );
  QDir dir(pDir);
    if(!dir.exists()){ 
      qDebug() << "Create parent Directory:" << dir.absolutePath();
      if( !dir.mkpath(dir.absolutePath()) ){
        qDebug() << fileName+": Could not create containing directory";
	return false;
      }
    }

  //Open the file with .tmp extension
  QFile file(fileName+".tmp");
  if( !file.open(QIODevice::WriteOnly | QIODevice::Text) ){
    qDebug() << fileName+".tmp: Failure -- Could not open file";
    return false;
  }
  //Write the file
  QTextStream ofile(&file); //start the output stream
  for(int i=0; i<contents.length(); i++){
    ofile << contents[i];
    ofile << "\n";
  }
  //Close the File
  file.close();
  //Remove any existing file with the final name/location
  if( QFile::exists(fileName) ){
    if( !QFile::remove(fileName) ){
      qDebug() << fileName+": Error -- Could not overwrite existing file";
      QFile::remove(fileName+".tmp");
      return false;
    }
  }
  //Move the temporary file into its final location
  if( !file.rename(fileName) ){
    qDebug() << fileName+": Error: Could not rename "+fileName+".tmp as "+fileName;
    return false;
  }
  //Return success
  QString extra = QDir::homePath(); //remove this from the filename display
  qDebug() << "Saved:" << fileName.replace(extra,"~");
  return true;;
}

bool PortUtils::removeFile(QString fileName){
  return PortUtils::createFile(fileName, QStringList() );
}

QStringList PortUtils::readFile(QString filePath){
  QStringList contents;
  //Check that the file exists first
  if(!QFile::exists(filePath)){ 
    qDebug() << "Warning: file to read does not exist:" << filePath;
    return contents; 
  }
  //Open the file for reading
  QFile file(filePath);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){ 
    qDebug() << "Error: file could not be opened for reading:"<< filePath;
    return contents; 
  }
  QTextStream in(&file);
  //Save the contents as a QStringList
  while(!in.atEnd()){
    contents << in.readLine();
  }
  //Return the contents (one entry per line)
  return contents;
}

