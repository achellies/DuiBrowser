/*
Copyright (C) 2006,2009 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//-----------------------------------------------------------------------------
// CallTargetIfOutOfDate.cs
//
// NAnt custom task which creates a bulkbuild '.cpp' file for every given
// input fileset.
//
// Required:
//	FileSetName    - Name of fileset containing files to be added to the bulk 'unit'.
//  OutputFilename - Filename for generated CPP.
//
// Optional
//	OutputDir	- This is where the BuildBuild files get generated to.
//
//-----------------------------------------------------------------------------

using NAnt.Core;
using NAnt.Core.Attributes;
using NAnt.Core.Logging;

using System;
using System.Collections.Generic;
using System.IO;


namespace EA.CallTargetIfOutOfDate
{
	[TaskName("CallTargetIfOutOfDate")]
	public class calltargetifoutofdate : Task
    {
        #region Private Instance Fields

        string _inputFileSetName = null;
        string _outputFileSetName = null;
        string _dummyOutputFileName = null;
        string _dependencyFileName = null;
        string _targetName = null;

        #endregion Private Instance Fields

        #region Public Instance Properties

        [TaskAttribute("InputFileset", Required = true)]
        public string InputFileSetName
		{
            get { return _inputFileSetName; }
            set { _inputFileSetName = value; }
		}

        [TaskAttribute("OutputFileset", Required = false)]
        public string OutputFileSetName
        {
            get { return _outputFileSetName; }
            set { _outputFileSetName = value; }
        }

        [TaskAttribute("DummyOutputFile", Required = false)]
        public string DummyOutputFileName
		{
            get { return _dummyOutputFileName; }
            set { _dummyOutputFileName = value; }
		}

        [TaskAttribute("DependencyFile", Required = true)]
        public string DependencyFileName
        {
            get { return _dependencyFileName; }
            set { _dependencyFileName = value; }
        }

        [TaskAttribute("TargetName", Required = true)]
        public string TargetName
        {
            get { return _targetName; }
            set { _targetName = value; }
        }

        #endregion Public Instance Properties

        #region static utility functions

        public static bool SortedListsEqual(List<string> listA, List<string> listB)
        {
            if (listA.Count != listB.Count)
            {
                return false;
            }

            IEnumerator<string> a = listA.GetEnumerator();
            IEnumerator<string> b = listB.GetEnumerator();
            while (a.MoveNext() && b.MoveNext())
            {
                if (0 != string.Compare(a.Current, b.Current, true))
                {
                    return false;
                }
            }
            return true;
        }

        /// <summary>Reads list of files from dependency file</summary>
        /// <returns>A sorted List containing all files listed in the dependency file</returns>
        public static List<string> ReadDependencyFile(TextReader dependencyFile)
        {
            List<string> files = new List<string>();
            string line;
            while ((line = dependencyFile.ReadLine()) != null)
            {
                line = line.TrimEnd(new char[] { '\r', '\n' });
                if (line.Length > 0)
                {
                    files.Add(line);
                }
            }
            files.Sort();
            return files;
        }

        /// <summary>Writes list of files to a dependency file</summary>
        public static void WriteDependencyFile(TextWriter dependencyFile, List<string> files)
        {
            foreach (string file in files)
            {
                if (file.Length > 0)
                {
                    dependencyFile.WriteLine(file);
                }
            }
        }

        public static List<string> GetFileListFromFileSet(FileSet fileSet)
        {
            List<string> files = new List<string>();
            foreach (FileItem file in fileSet.FileItems)
            {
                files.Add(file.FileName);
            }
            return files;
        }

        #endregion static utility functions

        /// <summary>Determine if the current and previous list of input files differ</summary>
        /// <returns><c>true</c> if the lists are not identical</returns>
        public bool InputDependencyListChanged(List<string> inputFiles, List<string> previousInputFiles)
        {
            if (previousInputFiles.Count != inputFiles.Count)
            {
                return true;
            }

            previousInputFiles.Sort();
            inputFiles.Sort();

            if (!SortedListsEqual(previousInputFiles, inputFiles))
            {
                return true;
            }

            return false;
        }

        /// <summary>Determine if the task needs to run.</summary>
        /// <returns><c>true</c> if we should run the program (dependents missing or not up to date), otherwise <c>false</c>.</returns>
        public bool TaskNeedsRunning(List<string> inputFiles, List<string> outputFiles, List<string> previousInputFiles)
        {
            // force rebuild if either input or output list is empty
            if (inputFiles.Count < 1)
            {
                return true;
            }
            if (outputFiles.Count < 1)
            {
                return true;
            }

            // force rebuild if dependency list has changed
            if (InputDependencyListChanged(inputFiles, previousInputFiles))
            {
                return true;
            }

            // force rebuild if any output file is missing
            foreach (string outputFile in outputFiles)
            {
                if (!File.Exists(outputFile))
                {
                    Log.WriteLineIf(Verbose, LogPrefix + "'{0}' does not exist.", outputFile);
                    return true;
                }
            }

            // force rebuild if any output file is older than any input file
            IEnumerator<string> currentInputFile = inputFiles.GetEnumerator();
            bool first = true;
            DateTime newestInputFileDateStamp = new DateTime();
            string newestFile;

            foreach (string outputFile in outputFiles)
            {
                if (!File.Exists(outputFile))
                {
                    Log.WriteLineIf(Verbose, "'{0}' does not exist.", outputFile);
                    return true;
                }

                // rebuild if outputDateStamp is older than any input file
                DateTime outputDateStamp = File.GetLastWriteTime(outputFile);

                while (currentInputFile.MoveNext())
                {
                    DateTime inputFileDateStamp = File.GetLastWriteTime(currentInputFile.Current);
                    if (!first && inputFileDateStamp <= newestInputFileDateStamp)
                    {
                        continue;
                    }
                    first = false;
                    newestInputFileDateStamp = inputFileDateStamp;
                    newestFile = currentInputFile.Current;

                    if (outputDateStamp <= newestInputFileDateStamp)
                    {
                        Log.WriteLineIf(Verbose, LogPrefix + "'{0}' is newer than '{1}'.", newestFile, outputFile);
                        return true;
                    }
                }
            }

            return false;
        }

        private List<string> GetOutputFiles()
        {
            List<string> outputFiles = new List<string>();
            if (OutputFileSetName != null && OutputFileSetName != string.Empty)
            {
                outputFiles.AddRange(GetFileListFromFileSet(Project.NamedFileSets[OutputFileSetName]));
            }

            if (DummyOutputFileName != null && DummyOutputFileName != string.Empty)
            {
                outputFiles.Add(DummyOutputFileName);
            }

            return outputFiles;
        }

        private List<string> GetInputFiles()
        {
            return GetFileListFromFileSet(Project.NamedFileSets[InputFileSetName]);
        }

        /// <summary>Execute the task.</summary>
		protected override void ExecuteTask()
		{
            // check input parameters
            // TODO: should this be moved to InitializeTask()?
            bool outputFileSetSpecified = (OutputFileSetName != null && OutputFileSetName != string.Empty);
            bool dummyOutputFileSpecified = (DummyOutputFileName != null && DummyOutputFileName != string.Empty);
            if (!outputFileSetSpecified && !dummyOutputFileSpecified)
            {
                throw new BuildException("You must specify one of OutputFileset or DummyOutputFile", Location);
            }

            List<string> outputFiles = GetOutputFiles();
            List<string> inputFiles = GetInputFiles();
            List<string> previousInputFiles = new List<string>();

            if (!File.Exists(DependencyFileName))
            {
                Log.WriteLineIf(Verbose, LogPrefix + "'{0}' does not exist.", DependencyFileName);
            }
            else
            {
                using (TextReader dependencyFile = new StreamReader(DependencyFileName))
                {
                    previousInputFiles = ReadDependencyFile(dependencyFile);
                }

                if (!TaskNeedsRunning(inputFiles, outputFiles, previousInputFiles))
                {
                    Log.WriteLineIf(Verbose, LogPrefix + "up to date.");
                    return;
                }
            }

            // touch dummy output file
            if (dummyOutputFileSpecified)
            {
                // borrowed from the <touch> task
                using (FileStream fs = File.Create(DummyOutputFileName))
                {
                }
                File.SetLastWriteTime(DummyOutputFileName, DateTime.Now);
            }

            try
            {
                // call target
                Target target = Project.Targets.Find(TargetName);
                if (target == null)
                {
                    throw new BuildException(String.Format("Unknown target '{0}'.", TargetName));
                }
                target.Copy().Execute();

                // write dependency file
                using (TextWriter dependencyFile = new StreamWriter(DependencyFileName))
                {
                    WriteDependencyFile(dependencyFile, inputFiles);
                }                
            }
            catch
            {
                Log.WriteLineIf(Verbose, LogPrefix + "failed.");

                // delete dummy output file if target build failed
                if (dummyOutputFileSpecified)
                {
                    File.Delete(DummyOutputFileName);
                }

                // re-throw the exception
                throw;
            }

            Log.WriteLineIf(Verbose, LogPrefix + "succeeded.");
        }
	}
}
