/*
Copyright (C) 2007,2009 Electronic Arts, Inc.  All rights reserved.

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
// SlnGenerator.cs
//
// Custom task which generates a composite SLN for a group of packages based
// on build file properties, using the <slnmaker> and <slnmerger> tasks.
//
// This is the core of the automated SLN workflow introduced in the
// "rwconfig-2.00.00 releases".
//
//-----------------------------------------------------------------------------

using System;
using System.Xml;
using System.Collections;
using System.Collections.Specialized;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;

using NAnt.Core;
using NAnt.Core.Tasks;
using NAnt.Core.Attributes;
using NAnt.Core.Util;
using EA.FrameworkTasks;
using EA.PackageSystem.PackageCore;
using EA.nantToVSTools;

namespace EA.SlnGenerator
{

    [TaskName("slngenerator")]
    public class SlnGenerator : Task
    { 
        string TARGET_GENERATE_SLN_ALL = "generate-sln-all";
        string TARGET_CSPROJ = "__private__csproj";
        string PROPERTY_SKIP_BUILD_NANTTOVSTOOLS = "skip-build-nanttovstools";
        Hashtable _properties = new Hashtable(); // to store all properties we used to put in initialize.xml
        Hashtable _projects = new Hashtable(); // to store project instance per package+config for performance
        Hashtable _packages = new Hashtable(); // to store packages of type EAPackage
        Hashtable _filesets = new Hashtable(); // to store all sln files that are needed to merge the final sln for each package
        Hashtable _native2crossmoduledependencies = new Hashtable(); // TO BE REMOVED
        ManagedModuleList _managedmodulelist = new ManagedModuleList(); // contains all managed modules

        XmlDocument _xmlDoc = new XmlDocument();
        XmlElement _targetElement;

        bool generateCompleteSlnsForAllPackages = false; // flag to tell if generate complete slns for all packages
        string _group; // indicate the current group(runtime, test, tool, example, all, allall)
        string _packagelist;
        string originalPackageConfigs = "";
        string _packagesCrossOnly = ""; // Forced cross only dependent package list for the current top level package
        string _packagesNativeOnly = ""; // Forced native only dependent package list for the current top level package
        static string [] _groups; // used when _group = 'all' or 'allall'
        [TaskAttribute("group", Required=false)]
        public string Group
        {
            get
            {
                if (_group != null)
                {
                    return _group;
                }
                return "runtime";
            }
            set 
            { 
                _group = value; 
            }
        }
        [TaskAttribute("packages", Required=false)]
        public string PackageList
        {
            get 
            {
                if(_packagelist != null)
                {
                    return _packagelist;
                }
                return "";
            }
            set
            {
                _packagelist = value;
            }
        }

        class ManagedModuleList : ArrayList
        {
            Hashtable indexer = new Hashtable();
            public ManagedModule Find(string modulekey)
            {
                if (!indexer.Contains(modulekey))
                    return null;
                else
                {
                    int index = (int)indexer[modulekey];
                    return (ManagedModule)base[index];
                }
            }
            public override int Add(object value)
            {
                if (value is ManagedModule)
                {
                    ManagedModule mm = (ManagedModule) value;
                    if (! indexer.Contains(mm.Package + "*" + mm.Config + "*" + mm.Module + "*" + mm.Group))
                    {
                        int index = base.Add(value);
                        indexer.Add(mm.Package + "*" + mm.Config + "*" + mm.Module + "*" + mm.Group, index);
                        return index;
                    }
                    return -1;
                }
                return -1;
            }
        }

        class ManagedModule
        {
            string package;
            string config;
            string module;
            string group;
            string vsprojlocation;
            bool isCSharp;
            bool isSingleModule;
            bool isCopyLocal;
            OptionSet projectReferences;
            public Hashtable dependentmodules;
            public string Package
            {
                get { return package; }
            }
            public string Config
            {
                get { return config; }
            }
            public string Module
            {
                get { return module; }
            }
            public string Group
            {
                get { return group; }
            }
            public string VSProjLocation
            {
                get { return vsprojlocation; }
            }
            public bool CopyLocal
            {
                get { return isCopyLocal; }
            }
            public bool IsCSharp
            {
                get { return isCSharp; }
            }
            public int DepModuleCount
            {
                get { return dependentmodules.Count; }
            }
            public int ProjRefCount
            {
                get { return projectReferences.Options.Count; }
            }
            public ManagedModule(string p, string c, string m, string g, bool iscsharp, bool issinglemodule, bool copylocal)
            {
                package = p;
                config = c;
                module = m;
                group = g;
                vsprojlocation = "";
                isCSharp = iscsharp;
                isSingleModule = issinglemodule;
                isCopyLocal = copylocal;
                dependentmodules = new Hashtable();
                projectReferences = new OptionSet();
            }
            public string CalculateVSProjPath(Project proj)
            {
                if (isCSharp)
                {
                    string projectName = module;
                    if (isSingleModule && !String.IsNullOrEmpty(group) && !group.Equals("runtime"))
                    {
                        projectName = group + "-" + module;
                    }
                    string csprojlocation = proj.Properties["package.builddir"] + Path.DirectorySeparatorChar
                        + projectName + Path.DirectorySeparatorChar + projectName + ".csproj";
                    if(proj.Properties.Contains("package." + package + ".designermode"))
                    {
                        if (proj.Properties["package." + package + ".designermode"] == "true")
                        {
                            string groupname = "";
                            if(isSingleModule)
                                groupname = group;
                            else
                                groupname = group + "." + module;
                            if(proj.Properties.Contains(groupname + ".resourcefiles.basedir"))
                            {
                                csprojlocation = proj.Properties[groupname + ".resourcefiles.basedir"] + Path.DirectorySeparatorChar + projectName + ".csproj";
                            }
                            else if(proj.NamedFileSets[groupname + ".sourcefiles"] != null)
                            {
                                csprojlocation = proj.NamedFileSets[groupname + ".sourcefiles"].BaseDirectory + Path.DirectorySeparatorChar + projectName + ".csproj";
                            }
                         }
                    }

                    if (config.IndexOf("vc-tool-") != -1)
                    {
                        string ext = Path.GetExtension(csprojlocation);
                        string fn = Path.GetFileNameWithoutExtension(csprojlocation);
                        string dir = Path.GetDirectoryName(csprojlocation);
                        csprojlocation = Path.Combine(dir, fn + "-tools" + ext);
                    }

                    vsprojlocation = csprojlocation;
                    return csprojlocation;
                }
                else
                {
                    string groupsubdir = "";
                    if(group != "runtime")
                        groupsubdir = Path.DirectorySeparatorChar + group;
                    string outputname = module;
                    if(config.IndexOf("cross") != -1)
                    {
                        if(module.IndexOf("cross") == -1)
                            outputname = "cross-" + module;
                    }
                    //string vcprojlocation = proj.Properties["package.configbuilddir"] + groupsubdir + Path.DirectorySeparatorChar + outputname + ".vcproj";
                    string vcprojlocation = proj.Properties["package.builddir"] + Path.DirectorySeparatorChar + outputname + ".vcproj";
                    vsprojlocation = vcprojlocation;
                    return vcprojlocation;
                }
            }
            public void AddDependentModules(string p, string c, string m, string g)
            {
                if (!dependentmodules.Contains(p + "*" + c + "*" + m + "*" + g))
                    dependentmodules.Add(p + "*" + c + "*" + m + "*" + g, "");
            }
            public void AddProjectReference(string module, string projlocation)
            {
                projectReferences.Options.Add(module, projlocation);
            }
            public void SetProjRefOptionSet(Project proj)
            {
                string groupname = "";
                if (isSingleModule)
                    groupname = group;
                else
                    groupname = group + "." + module;
                if (isCopyLocal)
                    proj.NamedOptionSets.Add("__private_" + groupname + "_copylocal_projectreferences", projectReferences);
                else
                    proj.NamedOptionSets.Add("__private_" + groupname + "_projectreferences", projectReferences);
            }
            public void RemoveProjRefOptionSet(Project proj)
            {
                string groupname = "";
                if (isSingleModule)
                    groupname = group;
                else
                    groupname = group + "." + module;
                if (isCopyLocal)
                {
                    proj.NamedOptionSets["__private_" + groupname + "_copylocal_projectreferences"] = null;
                    proj.NamedOptionSets.Remove("__private_" + groupname + "_copylocal_projectreferences");
                }
                else
                {
                    proj.NamedOptionSets["__private_" + groupname + "_projectreferences"] = null;
                    proj.NamedOptionSets.Remove("__private_" + groupname + "_projectreferences");
                }
            }
        }

        class EAPackage
        {
            public string pkgName;
            public string nativeConfigs;
            public string crossConfigs;
            public Hashtable crossConfigsExplored;
            public string crossOnly;
            public string nativeOnly;
            public string slnTarget;
            public Hashtable groupModules;
            public Hashtable dependents; // all build dependents for the package
            public bool autoBuildClean;
            public void UpdateExploredCrossConfigs(string nativeConfig, string crossConfig)
            {
                if(crossConfigsExplored == null)
                    crossConfigsExplored = new Hashtable();
                if(ListContainsString(crossConfigs, crossConfig))
                {
                    if(! crossConfigsExplored.Contains(nativeConfig))
                    {
                        foreach(string nconfig in crossConfigsExplored.Keys)
                        {
                            string cconfig = (string) crossConfigsExplored[nconfig];
                            if(cconfig == crossConfig)
                            {
                                crossConfigsExplored.Add(nativeConfig, "");
                                return;
                            }
                        }
                        crossConfigsExplored.Add(nativeConfig, crossConfig);
                    }
                }
            }
            public string GetExploredCrossConfig(string nativeConfig)
            {
                if(crossConfigsExplored == null)
                    return "";
                else if(crossConfigsExplored.Contains(nativeConfig))
                    return ((string) crossConfigsExplored[nativeConfig]);
                else
                    return "";
            }
            public bool UpdatePackageConfigs(string config)
            {
                if(config.Trim().IndexOf("cross") != -1)
                {
                    if(crossConfigs == "")
                    {
                        crossConfigs = config + " ";
                    }
                    else if(crossConfigs.IndexOf(config + " ") != -1)
                    {
                        return false;
                    }
                    else
                    {
                        crossConfigs = crossConfigs + config + " ";
                    }
                }

                else
                {
                    if(nativeConfigs == "")
                    {
                        nativeConfigs = config + " ";
                    }
                    else if(nativeConfigs.IndexOf(config + " ") != -1)
                    {
                        return false;
                    }
                    else
                    {
                        nativeConfigs = nativeConfigs + config + " ";
                    }
                }
                return true;
            }
            // for each group + config, store its build dependents
            public void UpdateDependents(string deplist, string config, string group)
            {
                if(dependents == null)
                {
                    dependents = new Hashtable();
                    foreach(string grp in _groups)
                    {
                        dependents.Add(grp, new Hashtable());
                    }
                }
                ((Hashtable)dependents[group])[config] = deplist;
            }
            // for each group + config, if its builddependents list contains 'dependent', then expand it
            // with the 'expandedListForDependent', which contains the complete list of builddependents for 'dependent'
            public void ExpandDependent(string dependent, string expandedListForDependent, string config)
            {
                if(dependents != null)
                {
                    foreach(string grp in _groups)
                    {
                        if(((Hashtable)dependents[grp]) != null)
                        {
                            string list = (string) ((Hashtable)dependents[grp])[config];
                            if(list != null)
                            {
                                if(ListContainsString(list, dependent))
                                {
                                    list = CombineStrings(list, expandedListForDependent);
                                    ((Hashtable)dependents[grp])[config] = list;
                                }
                            }
                        }
                    }
                }
            }
        }

        EA.FrameworkTasks.NAntTask CreateNantTask(string pkgname, string target, string config, string pkgconfigs, bool topLevel, Project parentProject)
        {
            EA.FrameworkTasks.NAntTask nantTask = new EA.FrameworkTasks.NAntTask();
            nantTask.Project = parentProject; // parent project
            if(pkgname == Project.Properties["package.name"] || config.IndexOf("cross") != -1)
                nantTask.BuildFileName = Path.Combine(Project.GetPropertyValue("package." + pkgname + ".dir"), 
                    pkgname + ".build");
            else
                nantTask.BuildFileName = Path.Combine(Project.GetPropertyValue("package." + pkgname + ".dir." + config), 
                    pkgname + ".build");
            nantTask.DefaultTarget = target;

            // For top level project don't want global properties to be readonly 
            string gProperties = parentProject.GlobalProperties;
            PackageXml px = PackageMap.Instance.GetMasterPackage(pkgname);
            string pxProperties = String.Empty;
            if (px != null)
            {
                 pxProperties = px.Properties;
            }
            
            if (topLevel)
            {
                parentProject.GlobalProperties = String.Empty;
                if (px != null)
                {
                    px.Properties = String.Empty;
                }
            }

            nantTask.DoInitialize();

            if (topLevel)
            {
                //Restore original values:
                parentProject.GlobalProperties = gProperties;
                if (px != null)
                {
                    px.Properties = pxProperties;
                }


                nantTask.NestedProject.GlobalProperties = gProperties;
                if (gProperties != string.Empty)
                {
                    foreach (string gprop in NantToVSTools.TrimAndSplit(gProperties))
                    {
                        if (parentProject.Properties.Contains(gprop) && !nantTask.NestedProject.Properties.Contains(gprop))
                        {
                            nantTask.NestedProject.Properties.Add(gprop, parentProject.Properties[gprop]);
                        }
                    }
                }
            }

            nantTask.NestedProject.Properties.AddReadOnly("config", config);
            nantTask.NestedProject.Properties.AddReadOnly("package.configs", pkgconfigs);

            if (parentProject.Properties.Contains(Project.NANT_PROPERTY_PROJECT_MASTERTARGET))
            {
                nantTask.NestedProject.Properties.UpdateReadOnly(Project.NANT_PROPERTY_PROJECT_MASTERTARGET, Project.Properties[Project.NANT_PROPERTY_PROJECT_MASTERTARGET]);
            }

            if (Project.Properties.Contains(Project.NANT_PROPERTY_PROJECT_CMDTARGETS))
            {
                nantTask.NestedProject.Properties.UpdateReadOnly(Project.NANT_PROPERTY_PROJECT_CMDTARGETS, Project.Properties[Project.NANT_PROPERTY_PROJECT_CMDTARGETS]);
            }

            if (topLevel)
            {
                // Passing down the bulkdbuild property from the origonal Project properties. 
                if (parentProject.Properties.Contains("bulkbuild"))
                    nantTask.NestedProject.Properties.Add("bulkbuild", Project.Properties["bulkbuild"]);
                else
                    nantTask.NestedProject.Properties.Add("bulkbuild", "true");
            }
            else
            {
                // Passing down the bulkdbuild property from the origonal Project properties. 
                if (parentProject.Properties.Contains("bulkbuild"))
                    nantTask.NestedProject.Properties.AddReadOnly("bulkbuild", Project.Properties["bulkbuild"]);
                else
                    nantTask.NestedProject.Properties.AddReadOnly("bulkbuild", "true");
            }

            if (topLevel && px != null)
            {
                // Check for properties for building the package conditionally
                if (px.Properties != null && px.Properties != string.Empty)
                {
                    foreach (string prop in NantToVSTools.TrimAndSplit(px.Properties))
                    {
                        if (parentProject.Properties.Contains(prop))
                        {
                            nantTask.NestedProject.Properties.Add(prop, parentProject.Properties[prop]);
                        }
                    }
                }
            }
            return nantTask;
        }      
                        
        EAPackage CheckPackageAutoBuildability(EAPackage eapkg)
        {
            // It doesn't make sense to check top level package's autobuildability
            if(eapkg.pkgName != Project.Properties["package.name"])
            {
                string masterVersion = PackageMap.Instance.GetMasterVersion(eapkg.pkgName, Project);
                if (masterVersion == null)
                {
                    string msg = string.Format("ERROR:  You must specify a version for package '{0}' in your masterconfig.xml file.", eapkg.pkgName);
                    throw new BuildException(msg, Location);
                }
                Release info = PackageMap.Instance.Releases.FindByNameAndVersion(eapkg.pkgName, masterVersion);
                if(info.AutoBuildClean.ToLower() == "false")
                {
                    eapkg.autoBuildClean = false;
                }
            }
            return eapkg;
        }

        void AddPackageProperties(string packagename)
        {
            if(! Project.Properties.Contains(String.Format("package.{0}.dir", packagename)))
            {
                string masterVersion = PackageMap.Instance.GetMasterVersion(packagename, Project);
                if (masterVersion == null)
                {
                    string msg = string.Format("ERROR:  You must specify a version for package '{0}' in your masterconfig.xml file.", packagename);
                    throw new BuildException(msg, Location);
                }
                Release info = PackageMap.Instance.Releases.FindByNameAndVersion(packagename, masterVersion);
                Project.Properties.AddReadOnly(String.Format("package.{0}.dir", info.Name), info.Path);
            }
        }
        Target ProjectTargetFind(Project proj, string target)
        {
            if(proj.Targets.Find(target) == null)
            {
                throw new BuildException("Unknown target '" + target + "' in package '" + proj.Properties["package.name"] + "' !");
            }
            return proj.Targets.Find(target);
        }
        void PopulateInitializeProperties(string config, string pkgname, Project proj)
        {
            string buildModules = "";
            string spuBuildModules = ""; // ps3 only
            string configSystem = proj.Properties["config-system"];
            string tempGroup = Group;
            if(pkgname != Project.Properties["package.name"])
            {
                tempGroup = "runtime";
            }
            else
            {
                return;
            }

            string groupbuildmodules = "";
            if (proj.Properties.Contains(tempGroup + ".buildmodules"))
            {
                groupbuildmodules = proj.Properties[tempGroup + ".buildmodules"];
            }
            else if (proj.Properties.Contains(tempGroup + ".buildtype") || proj.Properties.Contains(tempGroup + ".builddependencies"))
            {
                groupbuildmodules = pkgname;
            }
            else
            {
                return;
            }

            foreach (string module in NantToVSTools.TrimAndSplit(groupbuildmodules))
            {
                if(module != string.Empty)
                {
                    string buildType = proj.Properties[tempGroup + "." + module + ".buildtype"];
                    if (buildType == null && proj.Properties.Contains(tempGroup + ".buildtype"))
                    {
                        buildType = proj.Properties[tempGroup + ".buildtype"];
                    }
                    if(buildType == "Library")
                    {
                        buildType = "StdLibrary";
                    }
                    else if (buildType == "Program")
                    {
                        buildType = "StdProgram";
                    }
                    else if(buildType == null)
                        throw new BuildException("Error: module '" + module + "' in " + pkgname + " did not specify its build type!");
                    
                    if(configSystem.ToLower() == "ps3")
                    {
                        string spuCompiler = proj.NamedOptionSets["ps3spu"].Options["cc"];
                        string currCompiler = "";
                        if(proj.NamedOptionSets[buildType] == null)
                            Console.WriteLine("WARNING: Unknown build type: " + buildType + " for module '" + module + "' in " + pkgname + "!");
                        // this is how to determine the current module is SPU module or not.
                        if(proj.NamedOptionSets[buildType].Options.Contains("cc"))
                        {
                            currCompiler = proj.NamedOptionSets[buildType].Options["cc"];
                        }
                        if(currCompiler.Equals(spuCompiler))
                        {
                            spuBuildModules = spuBuildModules + " " + module;
                        }
                        else
                        {
                            buildModules = buildModules + " " + module;
                        }
                        if (buildType != null && proj.NamedOptionSets[buildType].Options["build.tasks"].IndexOf("lib") != -1)
                        {
                            _properties["package." + pkgname + "." + module + ".islibrary." + config] = "StdLibrary";
                        }
                    }
                    else
                    {
                        buildModules = buildModules + " " + module;
                    }

                    if (buildType != null)
                    {
                        string basetype = "";
                        if (IsModuleManaged(proj, tempGroup, module, ref basetype))
                        {
                            if(basetype.StartsWith("CSharp"))
                                _properties["package." + pkgname + ".runtime." + module + ".vsprojtype." + config] = "csproj";
                        }
                    }
                }
            }
            _properties["package." + pkgname + ".runtime.buildmodules." + config] = buildModules;
            if(spuBuildModules != "")
            {
                _properties["package." + pkgname + ".runtime.buildmodules.spu." + config] = spuBuildModules;
                _properties["package." + pkgname + ".runtime.buildmodules." + config] = buildModules + " " + spuBuildModules;
            }
        }

        private bool IsModuleDefined(Project proj, string group, string module)
        {
            string buildModules = "";
            if (proj.Properties.Contains(group + ".buildmodules"))
            {
                buildModules = proj.Properties[group + ".buildmodules"];
            }
            else if (proj.Properties.Contains(group + ".buildtype") || proj.Properties.Contains(group + ".builddependencies"))
            {
                buildModules = proj.Properties["package.name"];
            }
            if (ListContainsString(buildModules, module))
                return true;
            return false;
        }
        private bool IsModuleManaged(Project proj, string group, string module, ref string type)
        {
            string buildtype = "";
            if (proj.Properties.Contains(group + "." + module + ".buildtype"))
            {
                buildtype = proj.Properties[group + "." + module + ".buildtype"];
            }
            else if(proj.Properties.Contains(group + ".buildtype"))
            {
                buildtype = proj.Properties[group + ".buildtype"];
            }

            if (buildtype == "Library")
            {
                buildtype = "StdLibrary";
            }
            else if (buildtype == "Program")
            {
                buildtype = "StdProgram";
            }
            if (! proj.NamedOptionSets.Contains(buildtype))
            {
                throw new BuildException("Error: unrecognized build type '" + buildtype + "' for module '" + module + "' in '" + proj.Properties["package.name"] + "'!");
            }

            proj.Properties["slngenerator.moduletype"] = buildtype; 
            ProjectTargetFind(proj, "__private_get_module_basetype").Copy().Execute();
            string basetype = proj.Properties["GetModuleBaseType.RetVal"];
            if (basetype.StartsWith("CSharp"))
            {
                type = "CSharp";
                return true;
            }
            else if (basetype.StartsWith("Managed"))
            {
                type = "ManagedCpp";
                return true;
            }
            return false;
        }
        private string GetUnProcessedBuildModules(Project proj, string group, string config)
        {
            string buildModules = "";
            bool singleModule = false;
            string packagename = proj.Properties["package.name"];
            if (proj.Properties.Contains(group + ".buildmodules"))
            {
                buildModules = proj.Properties[group + ".buildmodules"];
                if (group.Equals("runtime"))
                    _properties[packagename + ".runtime.buildmodules.all." + config + ".vsproj"] = buildModules;

            }
            else if (proj.Properties.Contains(group + ".buildtype") || proj.Properties.Contains(group + ".builddependencies"))
            {
                buildModules = packagename;
                singleModule = true;
            }

            if(PackageList == "" || (! ListContainsString(PackageList, packagename)))
            {
                // Get the constraint build modules if the property exists
                if (_properties.Contains(packagename + "." + group + ".buildmodules." + config))
                {
                    string constraintBuildModules = (string)_properties[packagename + "." + group + ".buildmodules." + config];
                    if (constraintBuildModules != "")
                    {
                        buildModules = constraintBuildModules;
                    }
                    else
                    {
                        if(! singleModule)
                            _properties[packagename + "." + group + ".buildmodules." + config] = buildModules;
                    }
                }
                else
                    _properties[packagename + "." + group + ".buildmodules." + config] = "";
            }
            else
                _properties[packagename + "." + group + ".buildmodules." + config] = "";

            // Find Managed Modules
            foreach (string bm in NantToVSTools.TrimAndSplit(buildModules))
            {
                if (bm == String.Empty) continue;
                string basetype = "";
                if (IsModuleManaged(proj, group, bm, ref basetype))
                {
                    bool csharpModule = false;
                    if (basetype == "CSharp") csharpModule = true;
                    bool copylocal = false;
                    string copyLocalProperty = group + "." + bm + ".copylocal";
                    if (singleModule) copyLocalProperty = group + ".copylocal";
                    if (proj.Properties.Contains(copyLocalProperty))
                    {
                        if (proj.Properties[copyLocalProperty] == "true" || proj.Properties[copyLocalProperty] == "slim")
                            copylocal = true;
                    }
                    
                    ManagedModule mm = new ManagedModule(packagename, config, bm, group, csharpModule, singleModule, copylocal);
                    _managedmodulelist.Add(mm);
                }
            }

            return buildModules;
        }
        private bool IsBuildingEasharpCSC(Project proj)
        {
            // For easharp:
            return Project.Properties.Contains("easharp.runtime.build.buildcsc");
        }

        private bool FindInterModuleDependencies(Project proj, string group, string module, Hashtable newmodules)
        {
            string currentPackageName = proj.Properties["package.name"];
            string config = proj.Properties["config"];
            string bmConstrainProp = currentPackageName + "." + group + ".buildmodules." + config;
            ManagedModule managedModule = _managedmodulelist.Find(currentPackageName + "*" + config + "*" + module + "*" + group);
            if (_properties.Contains(bmConstrainProp))
            {
                string existingbmConstrain = (string)_properties[bmConstrainProp];
                if (existingbmConstrain != "")
                {
                    _properties[bmConstrainProp] = CombineStrings(existingbmConstrain, module);
                }
            }
            else
            {
                if (Project.Properties["package.name"] == currentPackageName)
                    _properties[bmConstrainProp] = "";
                else
                    _properties[bmConstrainProp] = module;
            }

            bool foundNative2CrossModuleDependency = false;
            string[] targetGroups = new String[] { "runtime", group };
            if (group == "runtime")
                targetGroups = new String[] { "runtime" };
            foreach (string dgroup in targetGroups)
            {
                string moduleDependenciesProperty = group + "." + module + "." + dgroup + ".moduledependencies";
                string moduleDependenciesPropertyPlatform = group + "." + module + "." + dgroup + ".moduledependencies." + proj.Properties["config-system"];
                string moduledependentlist = "";
                if (!proj.Properties.Contains(moduleDependenciesProperty))
                {
                    // check for singleModule
                    if (group != dgroup && proj.Properties.Contains(group + "." + dgroup + ".moduledependencies"))
                        moduledependentlist = proj.Properties[group + "." + dgroup + ".moduledependencies"];
                }
                else
                    moduledependentlist = proj.Properties[moduleDependenciesProperty];

                if(proj.Properties.Contains(moduleDependenciesPropertyPlatform))
                {
                    moduledependentlist = CombineStrings(moduledependentlist, proj.Properties[moduleDependenciesPropertyPlatform]);
                }
                else
                {
                    moduleDependenciesPropertyPlatform = group + "." + dgroup + ".moduledependencies." + proj.Properties["config-system"];
                    if(proj.Properties.Contains(moduleDependenciesPropertyPlatform))
                    {
                        moduledependentlist = CombineStrings(moduledependentlist, proj.Properties[moduleDependenciesPropertyPlatform]);
                    }
                }
                if (moduledependentlist != "")
                {
                    foreach (string mdependent in NantToVSTools.TrimAndSplit(moduledependentlist))
                    {
                        if(mdependent == String.Empty) continue;
                        // found cross -> native, error!
                        if (config.IndexOf("cross") != -1 && mdependent.IndexOf("cross") == -1)
                            throw new BuildException("\nError: cross module '" + module + "' can not depend on native module '" + mdependent +
                                "' in package '" + currentPackageName + "'\n\n" +
                                "If '" + mdependent + "' is a legitimate cross module, please prefix its name with 'cross-'!");

                        if (IsModuleDefined(proj, dgroup, mdependent)) // normal case, native->native cross->cross
                        {
                            newmodules[mdependent] = dgroup;
                            if (_managedmodulelist.Find(currentPackageName + "*" + config + "*" +  mdependent + "*" + dgroup) == null)
                            {
                                // See if 'nbm' is a managed module or not.
                                string basetype = "";
                                if (IsModuleManaged(proj, dgroup, mdependent, ref basetype))
                                {
                                    bool csharpModule = false;
                                    if (basetype == "CSharp") csharpModule = true;
                                    bool copylocal = false;
                                    if (proj.Properties.Contains(dgroup + "." + mdependent + ".copylocal"))
                                    {
                                        if (proj.Properties[dgroup + "." + mdependent + ".copylocal"] == "true" || proj.Properties[dgroup + "." + mdependent + ".copylocal"] == "slim")
                                            copylocal = true;
                                    }
                                    ManagedModule mm = new ManagedModule(currentPackageName, config, mdependent, dgroup, csharpModule, false, copylocal);
                                    _managedmodulelist.Add(mm);
                                    if (managedModule != null)
                                        managedModule.AddDependentModules(currentPackageName, config, mdependent, dgroup);
                                }
                            }
                            else
                            {
                                if (managedModule != null)
                                    managedModule.AddDependentModules(currentPackageName, config, mdependent, dgroup);
                            }

                            bool fn2cmd = FindInterModuleDependencies(proj, dgroup, mdependent, newmodules);
                            if (!foundNative2CrossModuleDependency)
                                foundNative2CrossModuleDependency = fn2cmd;
                        }
                        else if (config.IndexOf("cross") == -1 && mdependent.IndexOf("cross") != -1) // native -> cross
                        {
                            string crossConfig = proj.Properties["config-cross"] + "-" + proj.Properties["config-name"];
                            string bmConstrainProperty = currentPackageName + "." + dgroup + ".buildmodules." + crossConfig;
                            bool moduleAlreadyExist = false;
                            if (_properties.Contains(bmConstrainProperty))
                            {
                                string existingbmConstrain = (string)_properties[bmConstrainProperty];
                                if (existingbmConstrain != "")
                                {
                                    if(ListContainsString(existingbmConstrain, mdependent))
                                        moduleAlreadyExist = true;
                                    else
                                        _properties[bmConstrainProperty] = existingbmConstrain + " " + mdependent;
                                }
                                else
                                    moduleAlreadyExist = true;
                            }
                            else
                            {
                                if (Project.Properties["package.name"] == currentPackageName)
                                    _properties[bmConstrainProperty] = "";
                                else
                                    _properties[bmConstrainProperty] = mdependent;
                            }
                            // if the cross project is already created, then we dont need to explore it anymore
                            if (_projects.Contains(currentPackageName + "_" + crossConfig) && _projects[currentPackageName + "_" + crossConfig] != null)
                            {
                                Project crossProj = (Project)_projects[currentPackageName + "_" + crossConfig];
                                // Since cross config is already run, we can not rely on FindBuildDependencies to explore the dependents module info. 
                                // Instead we need to return this new cross module to the caller so that the caller can explore its dependents module info.
                                if(! moduleAlreadyExist)
                                    newmodules[mdependent] = dgroup;
                                FindInterModuleDependencies(crossProj, dgroup, mdependent, newmodules);
                            }
                            else // if the cross project is not created yet, we need to tell parents to auto explore it.
                                foundNative2CrossModuleDependency = true;
                        }
                        else
                        {
                            throw new BuildException("\nError: module '" + mdependent + "' is not defined in '" + dgroup + "' group for package '" + currentPackageName +
                                "' in '" + config + "' configuration!");
                        }
                    }
                }
            }
            return foundNative2CrossModuleDependency;
        }
        void CallPerModuleVcprojPrebuildTarget(Project proj, string grpname)
        {
            if (proj.Properties.Contains(grpname + ".vcproj.prebuildtarget"))
            {
                string perModuleVcprojPrebuildTarget = proj.Properties[grpname + ".vcproj.prebuildtarget"];
                Target target = proj.Targets.Find(perModuleVcprojPrebuildTarget);
                if (target != null)
                    target.Execute();
            }
        }
        void PopulateDependentBuildModules(string config, string group, Project proj, ref string needRunCrossConfig, ref string pkgsNeedExploreCross, ref string indirectBuilddependents, string buildModules)
        {
            string buildDependenciesProperty = "";
            string buildDependenciesPropertyPlatform = "";
            string selectedModulesProperty = "";
            string crossConfig = proj.Properties["config-cross"] + "-" + proj.Properties["config-name"];

            foreach(string bm in NantToVSTools.TrimAndSplit(buildModules))
            {
                if(bm != string.Empty)
                {
                    ManagedModule managedModule = _managedmodulelist.Find(proj.Properties["package.name"] + "*" + config + "*" + bm + "*" + group);
                    string groupname = group + "." + bm;
                    string dependentlist = "";
                    buildDependenciesProperty = group + "." + bm + ".builddependencies";
                    if(proj.Properties.Contains(buildDependenciesProperty))
                    {
                        dependentlist = proj.Properties[buildDependenciesProperty];
                    }
                    else
                    {
                        buildDependenciesProperty = group + ".builddependencies";
                        if(proj.Properties.Contains(buildDependenciesProperty))
                        {
                            groupname = group;
                            dependentlist = proj.Properties[buildDependenciesProperty];
                        }
                    }
                    buildDependenciesPropertyPlatform = group + "." + bm + ".builddependencies." + proj.Properties["config-system"];
                    if(proj.Properties.Contains(buildDependenciesPropertyPlatform))
                    {
                        dependentlist = CombineStrings(dependentlist, proj.Properties[buildDependenciesPropertyPlatform]);
                    }
                    else
                    {
                        buildDependenciesPropertyPlatform = group + ".builddependencies." + proj.Properties["config-system"];
                        if(proj.Properties.Contains(buildDependenciesPropertyPlatform))
                        {
                            dependentlist = CombineStrings(dependentlist, proj.Properties[buildDependenciesPropertyPlatform]);
                        }
                    }
                    // call per module vcproj prebuildtarget
                    CallPerModuleVcprojPrebuildTarget(proj, groupname);

                    foreach(string dependent in NantToVSTools.TrimAndSplit(dependentlist))
                    {
                        if(dependent != string.Empty)
                        {
                            bool hasSelectedModules = false;
                            bool singleModule = false;
                            selectedModulesProperty = group + "." + bm + "." + dependent + ".runtime.buildmodules";
                            if(proj.Properties.Contains(selectedModulesProperty))
                                hasSelectedModules = true;
                            else 
                            {
                                selectedModulesProperty = group + "." + dependent + ".runtime.buildmodules";
                                if(proj.Properties.Contains(selectedModulesProperty))
                                {
                                    hasSelectedModules = true;
                                    singleModule = true;
                                }
                            }

                            bool dependentProjAlreadyExist = false;
                            Project dependentProj = null;
                            if (_projects.Contains(dependent + "_" + config))
                            {
                                dependentProjAlreadyExist = true;
                                dependentProj = (Project)_projects[dependent + "_" + config];
                            }
                            string newModules = "";
                            string modulesInSameConfig = "";
                            string modulesInDiffConfig = "";

                            if (!hasSelectedModules)// it means the current package depends on all modules of this dependent for this configuration
                            {
                                if (_properties.Contains(dependent + ".runtime.buildmodules." + config) && dependentProjAlreadyExist)
                                {
                                    // figure out the new modules
                                    string existingModules = (string)_properties[dependent + ".runtime.buildmodules." + config];
                                    if (existingModules != "")
                                    {
                                        _properties[dependent + ".runtime.buildmodules." + config] = "";
                                        string allModules = GetUnProcessedBuildModules(dependentProj, "runtime", config);
                                        foreach (string sbm in NantToVSTools.TrimAndSplit(allModules))
                                        {
                                            if (sbm != string.Empty)
                                            {
                                                if (!ListContainsString(existingModules, sbm))
                                                {
                                                    newModules = newModules + " " + sbm;
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                    _properties[dependent + ".runtime.buildmodules." + config] = "";

                                if (managedModule != null)
                                {
                                    managedModule.AddDependentModules(dependent, config, "", "runtime");
                                }
                            }
                            else // it has selected modules
                            {
                                string sModules = (string)proj.Properties[selectedModulesProperty];
                                foreach (string sbm in NantToVSTools.TrimAndSplit(sModules))
                                {
                                    if (sbm != string.Empty)
                                    {
                                        if (sbm.StartsWith("cross-") && config.IndexOf("cross") == -1)
                                            modulesInDiffConfig = modulesInDiffConfig + " " + sbm;
                                        else
                                        {
                                            modulesInSameConfig = modulesInSameConfig + " " + sbm;
                                        }
                                    }
                                }
                                if (!_properties.Contains(dependent + ".runtime.buildmodules." + config))
                                {
                                    _properties[dependent + ".runtime.buildmodules." + config] = modulesInSameConfig;
                                }
                                else
                                {
                                    string modules = (string)_properties[dependent + ".runtime.buildmodules." + config];
                                    if (modules != "")
                                    {
                                        foreach (string sbm in NantToVSTools.TrimAndSplit(modulesInSameConfig))
                                        {
                                            if (sbm != string.Empty)
                                            {
                                                if (!ListContainsString(modules, sbm))
                                                {
                                                    newModules = newModules + " " + sbm;
                                                }
                                            }
                                        }
                                    }
                                    if (!dependentProjAlreadyExist)
                                        _properties[dependent + ".runtime.buildmodules." + config] = CombineStrings(modules, newModules);
                                }

                                if (managedModule != null)
                                {
                                    foreach (string m in NantToVSTools.TrimAndSplit(modulesInSameConfig))
                                    {
                                        if (m == String.Empty) continue;
                                        // we add it to temporary store because we don't know if they are managed module yet at this point.
                                        managedModule.AddDependentModules(dependent, config, m, "runtime");
                                    }
                                }
                            }

                            // if dependent proj already exist
                            if (dependentProjAlreadyExist && newModules != "")
                            {
                                if (dependentProj != null)
                                {
                                    Hashtable nmHash = new Hashtable();
                                    foreach (string nbm in NantToVSTools.TrimAndSplit(newModules))
                                    {
                                        if (nbm == String.Empty) continue;
                                        if (IsModuleDefined(dependentProj, "runtime", nbm))
                                        {
                                            // See if 'nbm' is a managed module or not.
                                            string basetype = "";
                                            if (IsModuleManaged(dependentProj, "runtime", nbm, ref basetype))
                                            {
                                                bool csharpModule = false;
                                                if (basetype == "CSharp") csharpModule = true;
                                                bool copylocal = false;
                                                if (dependentProj.Properties.Contains("runtime." + nbm + ".copylocal"))
                                                {
                                                    if (dependentProj.Properties["runtime." + nbm + ".copylocal"] == "true" || dependentProj.Properties["runtime." + nbm + ".copylocal"] == "slim")
                                                        copylocal = true;
                                                }
                                                ManagedModule mm = new ManagedModule(dependent, config, nbm, "runtime", csharpModule, false, copylocal);
                                                _managedmodulelist.Add(mm);
                                            }
                                            // Explore all intermodule depependencies using the existing proj
                                            // and find possible native->cross inter-module dependencies.
                                            // also add this new module to the corresponding constraint buildmodule property
                                            bool foundNativeToCrossModuleDependency = FindInterModuleDependencies(dependentProj, "runtime", nbm, nmHash);
                                            if (foundNativeToCrossModuleDependency)
                                            {
                                                pkgsNeedExploreCross = pkgsNeedExploreCross + " " + dependent;
                                            }
                                        }
                                        else
                                            throw new BuildException("\nError: module '" + nbm + "' is not defined in 'runtime' group for package '" + dependent +
                                                    "' in '" + config + "' configuration!");
                                    }

                                    string newCrossModules = "";
                                    foreach (string nm in nmHash.Keys)
                                    {
                                        if (nm == string.Empty) continue;
                                        if (config.IndexOf("cross") != 1)
                                            newModules = CombineStrings(newModules, nm);
                                        else
                                        {
                                            if (nm.IndexOf("cross") != -1)
                                                newCrossModules = CombineStrings(newCrossModules, nm);
                                            else
                                                newModules = CombineStrings(newModules, nm);
                                        }
                                    }
                                    // need to populate dependent buildmodules for this new module because we missed them when dependent runs.
                                    string tempFlag = ""; // useless passthrough variable
                                    string extrabuilddependentlist = "";
                                    PopulateDependentBuildModules(config, "runtime", dependentProj, ref tempFlag,
                                        ref pkgsNeedExploreCross, ref extrabuilddependentlist, newModules);
                                    indirectBuilddependents = CombineStrings(indirectBuilddependents, extrabuilddependentlist);
                                    string builddependentlist_deppackage = GetBuildDependentList(dependentProj, "runtime", newModules);
                                    indirectBuilddependents = CombineStrings(indirectBuilddependents, builddependentlist_deppackage);
                                    // examine all new explored build dependents' initialize.xml
                                    DoDependentTaskOnPackages(dependentProj, builddependentlist_deppackage);
                                    if (newCrossModules != "")
                                    {
                                        // we know the cross proj will exist by now. otherwise we won't see this cross module.
                                        Project crossdependentproj = (Project)_projects[dependent + "_" + crossConfig];
                                        string bdlist = GetBuildDependentList(crossdependentproj, "runtime", newCrossModules);
                                        pkgsNeedExploreCross = CombineStrings(pkgsNeedExploreCross, bdlist);
                                        DoDependentTaskOnPackages(crossdependentproj, bdlist);
                                        PopulateDependentBuildModules(crossConfig, "runtime", crossdependentproj, ref tempFlag, ref pkgsNeedExploreCross, ref pkgsNeedExploreCross, newCrossModules);

                                    }
                                }
                            }

                            // Found inter-package native to cross dependencies 
                            if (modulesInDiffConfig != "")
                            {
                                // tell slnmaker this dependent is depended by a native module, so if the dependent is a cross only package
                                // and has zero-module in native config, slnmaker will still process it in native config
                                if (singleModule)
                                    _properties[group + "." + dependent + ".nativedependable." + config] = "true";
                                else
                                    _properties[group + "." + bm + "." + dependent + ".nativedependable." + config] = "true";
                                pkgsNeedExploreCross = pkgsNeedExploreCross + " " + dependent;

                                if (_properties.Contains(dependent + ".runtime.buildmodules." + crossConfig))
                                {
                                    string modules = (string)_properties[dependent + ".runtime.buildmodules." + crossConfig];
                                    if (modules != "")
                                    {
                                        _properties[dependent + ".runtime.buildmodules." + crossConfig] = CombineStrings(modules, modulesInDiffConfig);
                                    }
                                }
                                else
                                {
                                    _properties[dependent + ".runtime.buildmodules." + crossConfig] = modulesInDiffConfig;
                                }
                            }
                        }
                    }

                    Hashtable newModulesHash = new Hashtable();
                    bool ntrc = FindInterModuleDependencies(proj, group, bm, newModulesHash);
                    if(config.IndexOf("cross") != -1 && newModulesHash.Count != 0)
                    {
                        foreach(string ncm in newModulesHash.Keys)
                        {
                            if(ncm == string.Empty) continue;
                            if(ListContainsString(buildModules, ncm)) continue;
                            string ncmgroup = (string) newModulesHash[ncm];
                            if(ncmgroup != group) continue;
                            string tempflag = ""; // useless passthrough parameter
                            string bdlist = GetBuildDependentList(proj, ncmgroup, ncm);
                            indirectBuilddependents = CombineStrings(indirectBuilddependents, bdlist);
                            DoDependentTaskOnPackages(proj, bdlist);
                            PopulateDependentBuildModules(config, ncmgroup, proj, ref tempflag, ref pkgsNeedExploreCross, ref indirectBuilddependents, ncm);
                        }
                    }
                    else
                    {
                        foreach(string nm in newModulesHash.Keys)
                        {
                            if(nm == String.Empty) continue;
                            string nmgroup = (string) newModulesHash[nm];
                            if(nmgroup != group) continue;
                            string tempflag = ""; // useless passthrough parameter
                            if(nm.StartsWith("cross"))
                            {
                                // we know the cross proj will exist by now. otherwise we won't see this cross module.
                                Project crossproj = (Project) _projects[proj.Properties["package.name"] + "_" + crossConfig];
                                string bdlist = GetBuildDependentList(crossproj, nmgroup, nm);
                                pkgsNeedExploreCross = CombineStrings(pkgsNeedExploreCross, bdlist);
                                DoDependentTaskOnPackages(crossproj, bdlist);
                                PopulateDependentBuildModules(crossConfig, nmgroup, crossproj, ref tempflag, ref pkgsNeedExploreCross, ref pkgsNeedExploreCross, nm);
                            }
                            else
                            {
                                if(ListContainsString(buildModules, nm)) continue;
                                string bdlist = GetBuildDependentList(proj, nmgroup, nm);
                                indirectBuilddependents = CombineStrings(indirectBuilddependents, bdlist);
                                DoDependentTaskOnPackages(proj, bdlist);
                                PopulateDependentBuildModules(config, nmgroup, proj, ref tempflag, ref pkgsNeedExploreCross, ref indirectBuilddependents, nm);
                            }
                        }
                    }
                    if (needRunCrossConfig != "true")
                    {
                        if (ntrc)
                            needRunCrossConfig = "true";
                    }
                }
            }
       
            /// START: TO BE REMOVED
            // For backwards compatibility we still need to suppor this hack: 
            // A Hack to model the group module dependencies other than 'runtime' moduledependencies.
            // Need to be removed once we have the other group moduledependencies support in eaconfig
            if (proj.NamedOptionSets.Contains("config.targetoverrides"))
            {
                if (proj.NamedOptionSets["config.targetoverrides"].Options.Contains("sln" + group + "-slnmaker"))
                {
                    if (!proj.Properties.Contains("sln" + group + "-slnmaker.checked"))
                    {
                        string excludeFlag = proj.NamedOptionSets["config.targetoverrides"].Options["sln" + group + "-slnmaker"];
                        if (excludeFlag == "exclude" && proj.Properties.Contains("rwconfig-exclude-" + "sln" + group + "-slnmaker"))
                        {
                            if (proj.Targets.Find("sln" + group + "-slnmaker") != null)
                            {
                                string targetfile = proj.Targets.Find("sln" + group + "-slnmaker").Location.FileName;
                                XmlDocument xmlDoc = new XmlDocument();
                                try
                                {
                                    xmlDoc.Load(targetfile);
                                    string xpath = "//target[@name = \"" + "sln" + group + "-slnmaker\"]/slnmaker";
                                    XmlNode xmlNode = xmlDoc.DocumentElement.SelectSingleNode(xpath);
                                    GatherModuleDpdFromCustmizedSlnMaker(proj, config, group, xmlNode, ref needRunCrossConfig);
                                    proj.Properties.Add("sln" + group + "-slnmaker.checked", "");
                                }
                                catch (Exception e)
                                {
                                    string error = e.ToString();
                                }
                            }
                        }
                    }
                }
            }
            /// END: TO BE REMOVED
        }

        /// START: TO BE REMOVED
        void GatherModuleDpdFromCustmizedSlnMaker(Project proj, string config, string group, XmlNode xmlNode, ref string needRunCrossConfig)
        {
            string projName = proj.Properties["package.name"];
            XmlAttribute pathAttribute, nameAttribute, modulesAttribute, groupAttribute;
            XmlNodeList nodeList = xmlNode.SelectNodes("./project");
            for (int i = 0; i < nodeList.Count; i++)
            {
                if (!GetConditionAttribute(nodeList.Item(i), "project")) continue;

                pathAttribute = nodeList.Item(i).Attributes["path"];
                nameAttribute = nodeList.Item(i).Attributes["name"];
                modulesAttribute = nodeList.Item(i).Attributes["modules"];
                groupAttribute = nodeList.Item(i).Attributes["group"];
                if (groupAttribute == null && pathAttribute == null)
                {
                    if (modulesAttribute != null)
                    {
                        string bm = "";
                        if (nameAttribute == null || (nameAttribute != null && nameAttribute.InnerText == projName))
                        {
                            bm = proj.ExpandProperties(modulesAttribute.InnerText);
                        }
                        if (bm != "")
                        {
                            XmlAttribute pathDepAttribute, nameDepAttribute, modulesDepAttribute;
                            XmlNodeList oNodeListDep = nodeList.Item(i).SelectNodes("./depends");
                            // we assume all dependent modules belong to the group ${group} 
                            string crossmoduledependentlist = "";
                            string nativemoduledependentlist = "";
                            for (int j = 0; j < oNodeListDep.Count; j++)
                            {
                                pathDepAttribute = oNodeListDep.Item(j).Attributes["path"];
                                nameDepAttribute = oNodeListDep.Item(j).Attributes["name"];
                                modulesDepAttribute = oNodeListDep.Item(j).Attributes["modules"];
                                if (pathDepAttribute == null && nameDepAttribute == null && modulesDepAttribute != null)
                                {
                                    foreach (string dpm in NantToVSTools.TrimAndSplit(proj.ExpandProperties(modulesDepAttribute.InnerText)))
                                    {
                                        if (dpm == string.Empty) continue;
                                        if (dpm.StartsWith("cross-")) // if found a cross dependent module
                                            crossmoduledependentlist = crossmoduledependentlist + " " + dpm;
                                        else
                                            nativemoduledependentlist = nativemoduledependentlist + " " + dpm;
                                    }
                                }
                            }
                            if (crossmoduledependentlist != "")
                            {
                                needRunCrossConfig = "true";
                                foreach (string item in NantToVSTools.TrimAndSplit(bm))
                                {
                                    if (item == string.Empty) continue;
                                    string crossConfig = proj.Properties["config-cross"] + "-" + proj.Properties["config-name"];
                                    if (item.StartsWith("cross"))
                                    {
                                        string constrainProperty = projName + "." + group + ".buildmodules";
                                        if (_properties.Contains(constrainProperty + "." + config))
                                        {
                                            string existingModules = (string)_properties[constrainProperty + "." + config];
                                            if (existingModules != "") // if it's empty, then it means it depends on everything
                                            {
                                                _properties[constrainProperty + "." + config] = CombineStrings(existingModules, crossmoduledependentlist);
                                            }
                                        }
                                        else
                                        {
                                            _properties[constrainProperty + "." + config] = crossmoduledependentlist;
                                        }
                                    }
                                    else
                                    {
                                        // we assume in customized slnmaker only [example|tool|test] modules dependencies, no inter-group module dependenices except depending on 'runtime'
                                        if (_native2crossmoduledependencies.Contains(projName + "." + group + ".buildmodules." + config + ".md." + item))
                                        {
                                            _native2crossmoduledependencies[projName + "." + group + ".buildmodules." + config + ".md." + item] =
                                                ((string)_native2crossmoduledependencies[projName + "." + group + ".buildmodules." + config + ".md." + item]) + " " + crossmoduledependentlist;
                                        }
                                        else
                                            _native2crossmoduledependencies.Add(projName + "." + group + ".buildmodules." + config + ".md." + item, crossConfig + "." + group + "." + crossmoduledependentlist);
                                    }
                                }
                            }
                            if (nativemoduledependentlist != "") // this is only true when we have native [example|test|tool] module dependencies in customized slnmaker
                            {
                                if (config.IndexOf("cross") != -1)
                                    Console.WriteLine("WARNING: Cross2Native module dependency found! " + bm + " depend(s) on " + nativemoduledependentlist);
                                string constrainProperty = projName + "." + group + ".buildmodules";
                                if (_properties.Contains(constrainProperty + "." + config))
                                {
                                    string existingModules = (string)_properties[constrainProperty + "." + config];
                                    if (existingModules != "") // if it's empty, then it means it depends on everything
                                    {
                                        _properties[constrainProperty + "." + config] = CombineStrings(existingModules, nativemoduledependentlist);
                                    }
                                }
                                else
                                {
                                    _properties[constrainProperty + "." + config] = nativemoduledependentlist;
                                }
                            }
                        }
                    }
                }
            }
        }
        protected bool GetConditionAttribute(XmlNode node, string elementName)
        {
            XmlAttribute ifAttribute = node.Attributes["if"];
            XmlAttribute unlessAttribute = node.Attributes["unless"];

            bool ret = true;

            // Emulate the weird behavior of NAnt when both unless and if is present 
            if (ifAttribute != null && unlessAttribute != null)
            {
                //throw
            }
            else if (ifAttribute != null)
            {
                ret = Expression.Evaluate(Project.ExpandProperties(ifAttribute.InnerText));
            }
            else if (unlessAttribute != null)
            {
                ret = !Expression.Evaluate(Project.ExpandProperties(unlessAttribute.InnerText));
            }

            return ret;
        }
        void PopulateNativeToCrossBuildModuleConstrains(string cfg)
        {
            foreach (string nativeModuleProperty in _native2crossmoduledependencies.Keys)
            {
                // <package>.runtime.buildmodules.<config>.md.<module>
                Match m = Regex.Match(nativeModuleProperty, "^([^\\.]+)\\.([^\\.]+)\\.buildmodules\\.([^\\.]+)\\.md\\.([^\\.]+)$");
                if (m.Success)
                {
                    string package = m.Groups[1].ToString();
                    string group = m.Groups[2].ToString();
                    string constrainProperty = package + "." + group + ".buildmodules";
                    string config = m.Groups[3].ToString();
                    if (cfg.IndexOf("cross") == -1 && cfg != config)
                        continue;
                    string nativeModule = m.Groups[4].ToString();
                    string crossModuleList = (string)_native2crossmoduledependencies[nativeModuleProperty];
                    string crossconfig = "";
                    string dgroup = "";
                    Match match = Regex.Match(crossModuleList, "([^\\.]+)\\.([^\\.]+)\\.(.+)");
                    if (match.Success)
                    {
                        crossconfig = match.Groups[1].ToString();
                        dgroup = match.Groups[2].ToString();
                        if (cfg.IndexOf("cross") != -1 && crossconfig != cfg)
                            continue;
                        crossModuleList = match.Groups[3].ToString();
                    }
                    string existingNativeModules = (string)_properties[constrainProperty + "." + config];
                    if (existingNativeModules == null || existingNativeModules == "" || ListContainsString(existingNativeModules, nativeModule))
                    {
                        string allCrossModules = "";
                        foreach (string crossModule in NantToVSTools.TrimAndSplit(crossModuleList))
                        {
                            if (crossModule != string.Empty)
                            {
                                string crossModules = crossModule + " " + GetModuleDependencies(package, crossconfig, crossModule, dgroup);
                                allCrossModules = CombineStrings(allCrossModules, crossModules);
                            }
                        }
                        if (_properties.Contains(constrainProperty + "." + crossconfig))
                        {
                            string existingCrossModules = (string)_properties[constrainProperty + "." + crossconfig];
                            if (existingCrossModules != "") // if it's empty, then it means it depends on everything
                            {
                                _properties[constrainProperty + "." + crossconfig] = CombineStrings(existingCrossModules, allCrossModules);
                            }
                        }
                        else
                        {
                            _properties[constrainProperty + "." + crossconfig] = allCrossModules;
                        }
                    }
                }
            }
        }
        void PopulateModuleDependencies()
        {
            Hashtable tempproperties = (Hashtable)_properties.Clone();
            foreach (string property in tempproperties.Keys)
            {
                string propValue = (string)_properties[property];
                Match m = Regex.Match(property, "^([^\\.]+)\\.([^\\.]+)\\.buildmodules\\.([^\\.]+)$");
                if (m.Success)
                {
                    string package = m.Groups[1].ToString();
                    string group = m.Groups[2].ToString();
                    string config = m.Groups[3].ToString();
                    if (group != "runtime" && group != "example" && group != "test" && group != "tool")
                        continue;
                    if (_projects.Contains(package + "_" + config))
                    {
                        if (propValue != "")
                        {
                            if (package == Project.Properties["package.name"]) // no constrains on buildmodule for top level package
                                _properties[property] = "";
                            else if (PackageList != "" && ListContainsString(PackageList, package))
                            {
                                _properties[property] = "";
                            }
                            else
                            {
                                string newPropValue = "";
                                foreach (string module in NantToVSTools.TrimAndSplit(propValue))
                                {
                                    if (module == string.Empty) continue;
                                    string moduleDeptns = GetModuleDependencies(package, config, module, group);
                                    foreach (string mdptn in NantToVSTools.TrimAndSplit(moduleDeptns))
                                    {
                                        if (mdptn == string.Empty) continue;
                                        if ((!mdptn.StartsWith("cross") && config.IndexOf("cross") == -1)
                                            || (mdptn.StartsWith("cross") && config.IndexOf("cross") != -1))
                                        {
                                            if(group == "runtime")
                                                newPropValue = CombineStrings(newPropValue, mdptn);
                                        }
                                    }
                                    newPropValue = CombineStrings(newPropValue, module);

                                }
                                _properties[property] = newPropValue;
                            }
                        }
                    }
                }
            }
            tempproperties.Clear();
        }
        string GetModuleDependencies(string pkg, string config, string module, string group)
        {
            Project proj = (Project)_projects[pkg + "_" + config];
            if (proj != null)
            {
                string Modules = "";
                string moduleDependencyProperty = group + "." + module + ".runtime.moduledependencies";
                string moduleDependencyPropertyPlatform = group + "." + module + ".runtime.moduledependencies." + proj.Properties["config-system"];
                
                if (proj.Properties.Contains(moduleDependencyProperty) || proj.Properties.Contains(moduleDependencyPropertyPlatform))
                {
                    string dependentModules = "";
                    if(proj.Properties.Contains(moduleDependencyProperty))
                        dependentModules = proj.Properties[moduleDependencyProperty];
                    if(proj.Properties.Contains(moduleDependencyPropertyPlatform))
                        dependentModules = CombineStrings(dependentModules, proj.Properties[moduleDependencyPropertyPlatform]);
                    Modules = dependentModules;
                    foreach (string dm in NantToVSTools.TrimAndSplit(dependentModules))
                    {
                        if (dm == string.Empty)
                            continue;
                        string modules = GetModuleDependencies(pkg, config, dm, "runtime");
                        if (modules != "")
                            Modules = CombineStrings(modules, Modules);
                    }
                    proj = null;
                    return Modules;
                }
            }
            proj = null;
            return "";
        }
        /// END: TO BE REMOVED

        void CatchZeroModulePackage(string pkgname, string config, Project proj)
        {
            // This only applies to dependent packages
            if(pkgname != Project.Properties["package.name"])
            {
                if(! proj.Properties.Contains("runtime.buildmodules"))
                {
                    if((! proj.Properties.Contains("runtime.buildtype")) && (! proj.Properties.Contains("runtime.builddependencies")))
                    {
                        // We got a zero-module package for 'config'!
                        if(! _properties.Contains(pkgname + ".zeromodule." + config))
                        {
                            _properties[pkgname + ".zeromodule." + config] = "true";
                        }
                    }
                }
            }
            proj = null;
        }
        string SortConstrainedBuildModules(string constrainedbuildmodules, string originalbuildmodules, ref string supressedBuildModules )
        {
            string [] bmArrays = NantToVSTools.TrimAndSplit(originalbuildmodules);
            ArrayList bmArrayList = new ArrayList(bmArrays);
            ArrayList indexArrayList = new ArrayList();
            foreach(string cbm in NantToVSTools.TrimAndSplit(constrainedbuildmodules))
            {
                if(cbm == string.Empty) continue;
                indexArrayList.Add(bmArrayList.IndexOf(cbm));
            }
            indexArrayList.Sort();
            string sortedBuildModules = "";
            foreach(int index in indexArrayList.ToArray())
            {
                sortedBuildModules = sortedBuildModules + " " + ((string) bmArrayList[index]);
            }
            
            for (int i = 0; i < bmArrays.Length; i++)
            {
                if (!indexArrayList.Contains(i))
                {
                    supressedBuildModules += " " + bmArrays[i];
                }
            }

            return sortedBuildModules;
        }
        // Is a particular string present in the white-space delimited list of strings?
        public static bool ListContainsString(string stringList, string requiredString)
        {
            foreach(string str in NantToVSTools.TrimAndSplit(stringList))
            {
                if(str != string.Empty)
                {
                    if (str == requiredString)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        public static string CombineStrings(string oldStr, string newStr)
        {
            string returnStr = oldStr;
            foreach(string newItem in NantToVSTools.TrimAndSplit(newStr))
            {
                if(newItem != string.Empty)
                {
                    bool hasExisted = false;
                    foreach(string oldItem in NantToVSTools.TrimAndSplit(oldStr))
                    {
                        if((oldItem != string.Empty) && (oldItem.ToLower() == newItem.ToLower()))
                        {
                            hasExisted = true;
                            break;
                        }
                    }
                    if(! hasExisted)
                        returnStr = returnStr + " " + newItem;
                }
            }
            return returnStr;
        } 
        private bool ProceedConfigForPackage(string package, string config)
        {
            if(package != Project.Properties["package.name"])
            {
                if(! _properties.Contains("package." + package + ".autobuildclean"))
                {
                    string masterVersion = PackageMap.Instance.GetMasterVersion(package, Project);
                    if (masterVersion == null)
                    {
                        string msg = string.Format("ERROR:  You must specify a version for package '{0}' in your masterconfig.xml file.", package);
                        throw new BuildException(msg, Location);
                    }
                    Release info = PackageMap.Instance.Releases.FindByNameAndVersion(package, masterVersion);
                    if(info.AutoBuildClean.ToLower() == "false")
                    {
                        // Since this package is autobuildclean = false, so no vcproj will be generated for it
                        // any project in the final sln that has project dependency on it will not have this package's libs in the link line
                        // therefore, we need to tell target 'eaconfig-vcproj' to explicitly include the libs exposed by this package.
                        _properties["package." + package + ".autobuildclean"] = "false";
                        return false;
                    }
                    else
                        _properties["package." + package + ".autobuildclean"] = "true";
                }
                else if(((string)_properties["package." + package + ".autobuildclean"]) == "false")
                    return false;
            }

            if(config.IndexOf("cross") != -1)
            {
                if(ListContainsString(_packagesNativeOnly, package))
                    return false;
            }
            else
            {
                if(ListContainsString(_packagesCrossOnly, package))
                    return false;
            }
            return true;
        }

        private void PopulateProjectReferences(string config)
        {
            for (int i = 0; i < _managedmodulelist.Count; i++)
            {
                ManagedModule mm = (ManagedModule)_managedmodulelist[i];
                if (mm.Config == config)
                {
                    Project proj = (Project)_projects[mm.Package + "_" + config];
                    string projfilename = mm.CalculateVSProjPath(proj);
                    // we need to save the designer mode's csproj location because it's different from the normal csproj location
                    // so that slnmaker later on can access it
                    if (proj.Properties.Contains("package." + mm.Package + ".designermode"))
                        _properties["___private_" + mm.Package + "." + mm.Module + ".csproj_projroot." + config] = Path.GetDirectoryName(projfilename);

                    if (mm.DepModuleCount != 0 && mm.Config == config)
                    {
                        foreach (string depmodule in mm.dependentmodules.Keys)
                        {
                            ManagedModule depmm = _managedmodulelist.Find(depmodule);
                            if (depmm != null)
                            {
                                Project depproj = (Project)_projects[depmm.Package + "_" + config];
                                string dependentprojfilename = depmm.CalculateVSProjPath(depproj);
                                string relativeProjFileName = NantToVSTools.GetRelativeFileName(dependentprojfilename, Path.GetDirectoryName(projfilename));
                                mm.AddProjectReference(depmm.Module + " " + depmm.Package, relativeProjFileName);
                                depproj = null;
                            }
                            else // it might depend on all modules of a package
                            {
                                Match match = Regex.Match(depmodule, "([^\\*]+)\\*[^\\*]+\\*\\*([^\\*]+)");
                                if (match.Success)
                                {
                                    string deppackage = match.Groups[1].ToString();
                                    string depgroup = match.Groups[2].ToString();
                                    Project depproj = (Project)_projects[deppackage + "_" + config];
                                    foreach (ManagedModule m in _managedmodulelist)
                                    {
                                        if (m.Package == deppackage && m.Config == config && m.Group == depgroup)
                                        {
                                            string dependentprojfilename = m.CalculateVSProjPath(depproj);
                                            string relativeProjFileName = NantToVSTools.GetRelativeFileName(dependentprojfilename, Path.GetDirectoryName(projfilename));
                                            mm.AddProjectReference(m.Module + " " + deppackage, relativeProjFileName);
                                        }
                                    }
                                    depproj = null;
                                }
                            }
                        }
                    }
                    proj = null;
                }
            }
        }
        private void CallVcprojPrebuildTarget(Project proj)
        {
            if (proj.Properties.Contains("vcproj.prebuildtarget"))
            {
                string vcprojPrebuildTarget = proj.Properties["vcproj.prebuildtarget"];
                Target target = proj.Targets.Find(vcprojPrebuildTarget);
                if (target != null)
                    target.Execute();
            }
        }
        private void DoDependentTaskOnPackages(Project proj, string packagelist)
        {
            XmlElement dependentTaskElement = _xmlDoc.CreateElement("dependent");
            foreach (string package in NantToVSTools.TrimAndSplit(packagelist))
            {
                if (package == String.Empty) continue;
                dependentTaskElement.SetAttribute("name", package);
                _targetElement.RemoveAll();
                _targetElement.AppendChild(dependentTaskElement); // dependent task has to be embedded inside target task.
                DependentTask dependentTask = new DependentTask();
                dependentTask.Project = proj;
                dependentTask.PackageName = package;
                dependentTask.Initialize(_targetElement.FirstChild);
                dependentTask.Execute();
                dependentTask.Project = null;
                Project.Properties["package." + package + ".dir." + proj.Properties["config"]] = proj.Properties["package." + package + ".dir"];
            }
        }
        string FindBuildDependencies(string pkgname, string config, string configs, string group, bool toplevel, bool autoExploreMode, Project parentProject)
        {
            if (parentProject == null)
            {
                parentProject = Project;
            }

            string currGroup = group; // this variable is here because group might be 'all' or 'allall', which means we will run for more than one group
            if(toplevel && (group == "all" || group == "allall"))
            {
                currGroup = "runtime"; // if all or allall, run "runtime" first for top level package
            }
            else if(group == "allall")
            {
                currGroup = "runtime"; // only do the same to dependent package as toplevel package if 'allall' is specified.
            }
            EAPackage eapackage;
            if(_packages.Contains(pkgname))
            {
                eapackage = (EAPackage) _packages[pkgname];
                if(eapackage.UpdatePackageConfigs(config))
                {
                    _packages[pkgname] = eapackage;
                }
                else if(((Hashtable)eapackage.dependents[currGroup]) != null)
                {
                    if(((string)((Hashtable)eapackage.dependents[currGroup])[config]) != null)
                    {
                        return (string) ((Hashtable)eapackage.dependents[currGroup])[config];
                    }
                    else
                    {
                        return "";
                    }
                }
                else
                {
                    return "";
                }
            }
            else
            {
                eapackage = new EAPackage();
                eapackage.pkgName = pkgname;
                eapackage.autoBuildClean = true;
                eapackage.crossConfigs = "";
                eapackage.nativeConfigs = "";
                eapackage.slnTarget = null;
                eapackage.groupModules = new Hashtable();
                eapackage.UpdatePackageConfigs(config);
                _packages.Add(pkgname, eapackage);
            }

            EA.FrameworkTasks.NAntTask nantTask;
            Project nestedProject;
            // If we are already in the context of the current config for top level package, just execute the target
            // to save some time since instantiating and running a nantTask is time-consuming
            if(Project.Properties["config"] == config && toplevel && PackageList == "")
            {
                nantTask = null;
                ProjectTargetFind(Project, currGroup + "_builddependencies").Copy().Execute();
                nestedProject = Project;
            }
            else
            {
                nantTask = CreateNantTask(pkgname, currGroup + "_builddependencies", config, configs, true, parentProject);
                nantTask.ExecuteTaskNoDispose();
                nestedProject = nantTask.NestedProject;
            }
            nestedProject.Properties["package." + pkgname + ".slngeneratormode"] = "true";
            CallVcprojPrebuildTarget(nestedProject);

            // Find out whether this package + config has any runtime module defined
            CatchZeroModulePackage(pkgname, config, nestedProject);

            // Save the current project for generating vcproj and solutions later
            _projects[pkgname + "_" + config] = nestedProject;

            // Initialization for per package properties when the EAPackage object gets first created.
            if(eapackage.crossOnly == null && eapackage.nativeOnly == null)
            {
                // Find out if the package is cross-only or native-only or neither
                if(nestedProject.Properties.Contains("rwconfig.cross-only"))
                {
                    eapackage.crossOnly = nestedProject.Properties["rwconfig.cross-only"];
                }
                else
                {
                    eapackage.crossOnly = "";
                }
                if(nestedProject.Properties.Contains("rwconfig.native-only"))
                {
                    eapackage.nativeOnly = nestedProject.Properties["rwconfig.native-only"];
                }
                else
                {
                    eapackage.nativeOnly = "";
                }
                // For backward compatibility with rwconfig 1.x
                if(nestedProject.Properties.Contains("rwconfig.is-use-default-targets"))
                {
                    if(nestedProject.Properties.Contains("rwconfig.enableslngeneration"))
                    {
                        if(nestedProject.Properties["rwconfig.enableslngeneration"].ToLower() != "true")
                        {
                            if(nestedProject.Properties["rwconfig.is-use-default-targets"].ToLower() == "true")
                            {
                                eapackage.slnTarget = TARGET_GENERATE_SLN_ALL;
                            }
                        }
                    }
                    else if(nestedProject.Properties["rwconfig.is-use-default-targets"].ToLower() == "true")
                    {
                        eapackage.slnTarget = TARGET_GENERATE_SLN_ALL;
                    }
                }
                // See if the package is autobuildable
                eapackage = CheckPackageAutoBuildability(eapackage);
                _packages[pkgname] = eapackage;
            }
            // Let top level project know about all dependents' build directory and package directory in order to find all solution and vcproj files later
            Project.Properties["package." + pkgname + ".dir." + config] = nestedProject.Properties["package.dir"];
            Project.Properties["package." + pkgname + ".builddir." + config] = nestedProject.Properties["package.builddir"];
            // need to save this property as well for merged projects
            Project.Properties["package." + pkgname + ".builddir"] = nestedProject.Properties["package.builddir"];

            string needRunCrossConfig = "false";
            string dependentPackagesNeedExploreCross = "";
            string exploredIndirectBuilddependentList = "";
            // Prepare all properties that are used to be put in initialize.xml because slnmaker needs to know
            // these properties.
            PopulateInitializeProperties(config, pkgname, nestedProject);
            // Get all info about the dependent runtime buildmodules that the current package depends on.
            // This is to tell slnmaker to not include project dependencines on unused modules.
            string bms = GetUnProcessedBuildModules(nestedProject, currGroup, config);
            PopulateDependentBuildModules(config, currGroup, nestedProject, ref needRunCrossConfig, 
                ref dependentPackagesNeedExploreCross, ref exploredIndirectBuilddependentList, bms);

            string builddependentlist = GetBuildDependentList(nestedProject, currGroup, bms); 
            
            // examine all new explored build dependents' initialize.xml
            DoDependentTaskOnPackages(nestedProject, builddependentlist);

            builddependentlist = CombineStrings(builddependentlist, exploredIndirectBuilddependentList);
            eapackage.UpdateDependents(builddependentlist.Trim(), config, currGroup);
            _packages[pkgname] = eapackage;

            string groupPassedDown = "runtime";
            if(toplevel || ((group == "allall") && (!toplevel)))
            {
                // Get all build dependents for all groups for current package and current config
                foreach(string grp in _groups)
                {
                    if(grp != currGroup)
                    {
                        bms = GetUnProcessedBuildModules(nestedProject, grp, config);
                        exploredIndirectBuilddependentList = "";
                        PopulateDependentBuildModules(config, grp, nestedProject, ref needRunCrossConfig, ref dependentPackagesNeedExploreCross, ref exploredIndirectBuilddependentList, bms);
                        string groupbuilddependentlist = GetBuildDependentList(nestedProject, grp, bms);
                        // examine all new explored build dependents' initialize.xml
                        DoDependentTaskOnPackages(nestedProject, groupbuilddependentlist);

                        builddependentlist = CombineStrings(builddependentlist, exploredIndirectBuilddependentList);
                        builddependentlist = CombineStrings(builddependentlist, groupbuilddependentlist);//nestedProject.Properties[PROPERTY_BUILD_DEPENDENCIES]);
                        eapackage.UpdateDependents(builddependentlist, config, grp);
                        _packages[pkgname] = eapackage;
                    }
                }
                if(group == "allall")
                    groupPassedDown = group;
            }

            string fullDependentList = ""; // this will be returned to parent package.
            if(builddependentlist.Trim() != string.Empty)
            {
                foreach(string dependent in NantToVSTools.TrimAndSplit(builddependentlist))
                {
                    // If the dependent is framework 1.0 package, we dont proceed anymore.
                    if(nestedProject.Properties["package." + dependent + ".frameworkversion"] == "1")
                        continue;
                    // Do not proceed if the dependent package is set autobuildclean = false.
                    // Also check whether dependent is forced to be cross only or native only package and then see whether to proceed or not
                    // for the current config (here cross/native only doesn't mean dependent is cross/native-only package.)
                    if(! ProceedConfigForPackage(dependent, config))
                    {
                        if(_properties.Contains("package." + dependent + ".autobuildclean"))
                        {
                            if(((string)_properties["package." + dependent + ".autobuildclean"]) == "false")
                            {
                                nestedProject.Properties["package." + dependent + ".autobuildclean"] = "false";
                                _projects[pkgname + "_" + config] = nestedProject;
                            }
                        }
                        continue;
                    }
                    AddPackageProperties(dependent);
                    string deplist = FindBuildDependencies(dependent, config, configs, groupPassedDown, false, false, nestedProject);
                    if(deplist != "")
                    {
                        eapackage.ExpandDependent(dependent, deplist, config);
                        _packages[pkgname] = eapackage;
                    }
                }

                // Normally we only return runtime group dependents, but when auto-exploring is on, we need to get all dependents from all groups.
                if(autoExploreMode)
                {
                    // we only need to return dependents from all groups when the follwoing condition is true.
                    if(toplevel || ((group == "allall") && (!toplevel)))
                    {
                        foreach(string grp in _groups)
                        {
                            fullDependentList = CombineStrings(fullDependentList, (string) ((Hashtable)eapackage.dependents[grp])[config]);
                        }

                    }
                    else
                        fullDependentList = CombineStrings(fullDependentList, (string) ((Hashtable)eapackage.dependents[currGroup])[config]);
                }
                else
                {
                    fullDependentList = CombineStrings(fullDependentList, (string) ((Hashtable)eapackage.dependents[currGroup])[config]);
                }
            }

            // if we are in native config and we found native module depends on cross module, it means we need to run cross config as well
            if(config.IndexOf("cross") == -1)
            {
                // we found native module in one package depend on cross module in other package
                if(dependentPackagesNeedExploreCross != "")
                {
                    string crossConfig = nestedProject.Properties["config-cross"] + "-" + nestedProject.Properties["config-name"];
                    foreach(string deppkg in NantToVSTools.TrimAndSplit(dependentPackagesNeedExploreCross))
                    {
                        if(ProceedConfigForPackage(deppkg, crossConfig))
                        {
                            string plist = FindBuildDependencies(deppkg, crossConfig, configs, groupPassedDown, false, true, null);
                            foreach(string dpkg in NantToVSTools.TrimAndSplit(plist + " " + deppkg))
                            {
                                if(_packages.Contains(dpkg))
                                {
                                    EAPackage eadpkg = (EAPackage) _packages[dpkg];
                                    eadpkg.UpdateExploredCrossConfigs(config, crossConfig);
                                    _packages[dpkg] = eadpkg;
                                    // Add this cross config into package.configs unless it's there already
                                    string pConfigs = Project.Properties["package.configs"];
                                    if(! ListContainsString(pConfigs, crossConfig))
                                        Project.Properties.UpdateReadOnly("package.configs", pConfigs + " " + crossConfig);
                                }
                            }
                        }
                    }
                }
                if(needRunCrossConfig == "true")
                {
                    string crossConfig = nestedProject.Properties["config-cross"] + "-" + nestedProject.Properties["config-name"];
                    if(! ListContainsString(eapackage.crossConfigs, crossConfig)) // if this cross config hasn't been run yet
                    {
                        if(ProceedConfigForPackage(pkgname, crossConfig))
                        {
                            string plist = FindBuildDependencies(pkgname, crossConfig, configs, group, toplevel, true, null);
                            foreach(string dpkg in NantToVSTools.TrimAndSplit(plist))
                            {
                                if(_packages.Contains(dpkg))
                                {
                                    EAPackage eadpkg = (EAPackage) _packages[dpkg];
                                    eadpkg.UpdateExploredCrossConfigs(config, crossConfig);
                                    _packages[dpkg] = eadpkg;
                                }
                            }
                            eapackage = (EAPackage)_packages[eapackage.pkgName];
                            eapackage.UpdateExploredCrossConfigs(config, crossConfig);
                            // Add this cross config into package.configs unless it's there already
                            string pConfigs = Project.Properties["package.configs"];
                            if(! ListContainsString(pConfigs, crossConfig))
                                Project.Properties.UpdateReadOnly("package.configs", pConfigs + " " + crossConfig);
                        }
                    }
                }
            }

            if(nantTask != null)
            {
                // remove the reference to nantTask.nestedProject
                nestedProject.Dispose();
                nestedProject = null;
                nantTask.NestedProject.Dispose();
                nantTask.NestedProject = null;
                nantTask.Project = null;
                nantTask = null;
            }
            _packages[pkgname] = eapackage;
            return fullDependentList;
        }

        private string GetBuildDependentList(Project proj, string group, string modules)
        {
            bool isSingleModule = false;
            string builddependentlist = "";
            string platform = proj.Properties["config-system"];
            if (! proj.Properties.Contains(group + ".buildmodules"))
                isSingleModule = true;
            if (isSingleModule)
            {
                if (proj.Properties.Contains(group + ".builddependencies"))
                {
                    builddependentlist = proj.Properties[group + ".builddependencies"];
                }
                if (proj.Properties.Contains(group + ".builddependencies." + platform))
                {
                    builddependentlist = CombineStrings(builddependentlist, proj.Properties[group + ".builddependencies." + platform]);
                }
            }
            else
            {
                foreach (string module in NantToVSTools.TrimAndSplit(modules))
                {
                    if (module == String.Empty) continue;
                    if (proj.Properties.Contains(group + "." + module + ".builddependencies"))
                    {
                        builddependentlist = CombineStrings(builddependentlist, proj.Properties[group + "." + module + ".builddependencies"]);
                    }
                    if (proj.Properties.Contains(group + "." + module + ".builddependencies." + platform))
                    {
                        builddependentlist = CombineStrings(builddependentlist, proj.Properties[group + "." + module + ".builddependencies." + platform]);
                    }
                }
            }
            return builddependentlist;
        }
        
        // for each package, if its complete build dependent list contains 'pkg', then add 'pkg's sln to the 
        // package's fileset, which will be used as input to slnmerger later.
        void AddSlnToFileSets(string pkg, string group, string slnfilepath)
        {
            if (!File.Exists(slnfilepath))
                return;

            if(generateCompleteSlnsForAllPackages || PackageList != "")
            {
                foreach(object package in _packages.Keys)
                {
                    XmlElement includes = _xmlDoc.CreateElement("includes");
                    includes.SetAttribute("name", slnfilepath);
                    EAPackage eap = (EAPackage) _packages[package];
                    if(eap.pkgName == pkg)
                    {
                        ((XmlElement)_filesets[eap.pkgName]).AppendChild(includes);
                        continue;
                    }
                    string deplist = "";
                    foreach(string grp in _groups)
                    {
                        foreach(string cfg in ((Hashtable)eap.dependents[grp]).Keys)
                        {
                            if(((string)((Hashtable)eap.dependents[grp])[cfg]) != null)
                            {
                                deplist = CombineStrings(deplist, (string) ((Hashtable)eap.dependents[grp])[cfg]);
                            }
                        }
                    }
                    if (ListContainsString(deplist, pkg))
                    {
                        ((XmlElement)_filesets[eap.pkgName]).AppendChild(includes);
                    }
                }
            }
            else
            {
                XmlElement includes = _xmlDoc.CreateElement("includes");
                includes.SetAttribute("name", slnfilepath);
                EAPackage eap = (EAPackage) _packages[Project.Properties["package.name"]];
                if(eap.pkgName == pkg)
                {
                    ((XmlElement)_filesets[eap.pkgName]).AppendChild(includes);
                }
                else if(group != "runtime" && eap.pkgName != pkg)
                {
                    ((XmlElement)_filesets[eap.pkgName]).AppendChild(includes);
                }
                else
                {
                    string deplist = "";
                    foreach(string grp in _groups)
                    {
                        foreach(string cfg in ((Hashtable)eap.dependents[grp]).Keys)
                        {
                            if(((string)((Hashtable)eap.dependents[grp])[cfg]) != null)
                            {
                                deplist = CombineStrings(deplist, (string) ((Hashtable)eap.dependents[grp])[cfg]);
                            }
                        }
                    }
                    if(ListContainsString(deplist, pkg))
                        ((XmlElement)_filesets[eap.pkgName]).AppendChild(includes);
                }
            }
        }
        string MergeSolutions(string packagename, string packagebuilddir, bool toplevel)
        {
            try 
            {
                SlnMerger slnMergerTask = new SlnMerger(Project);
                FileSet fs = new FileSet();
                fs.Project = Project;
                fs.Initialize((XmlElement)_filesets[packagename]);
                slnMergerTask.Inputs = fs;
                if(Project.Properties.Contains("generate-single-config"))
                {
                    slnMergerTask.Output = packagebuilddir + Path.DirectorySeparatorChar + Project.Properties["config"] + Path.DirectorySeparatorChar + packagename + ".sln";
                }
                else if(Group == "all" || Group == "allall" || Group == "runtime")
                {
                    slnMergerTask.Output = packagebuilddir + Path.DirectorySeparatorChar + packagename + ".sln";
                }
                else
                {
                    if(toplevel)
                    {
                        slnMergerTask.Output = packagebuilddir + Path.DirectorySeparatorChar + packagename + "-" + Group + ".sln";
                    }
                    else
                    {
                        slnMergerTask.Output = packagebuilddir + Path.DirectorySeparatorChar + packagename + ".sln";
                    }
                }
                slnMergerTask.ExecuteSlnMergerTask();
                return slnMergerTask.Output;
            }
            catch(BuildException be)
            {
                throw new BuildException(be.ToString());
            }
        }
        void MergeVcprojs()
        {
            foreach(object package in _packages.Keys)
            {
                EAPackage eapkg = (EAPackage) _packages[package];
                if(eapkg.slnTarget != null) // rwconfig1.x package found, we need to merge all vcprojs generated by 'generate-sln-all'.
                {
                    string pkgBuildDir = Project.Properties["package." + eapkg.pkgName + ".builddir"];
                    string modulesToMerge = "";
                    foreach(string vcprojFile in Directory.GetFiles(pkgBuildDir + Path.DirectorySeparatorChar, "*.vcproj"))
                    {
                        modulesToMerge = CombineStrings(modulesToMerge, Path.GetFileNameWithoutExtension(vcprojFile));
                    }
                    foreach(string module in NantToVSTools.TrimAndSplit(modulesToMerge))
                    {
                        if(module == String.Empty)
                            continue;
                        string applicableConfigs = "";
                        foreach(string configdir in Directory.GetDirectories(pkgBuildDir + Path.DirectorySeparatorChar))
                        {
                            string tempconfigdir = configdir.Replace("\\", Path.DirectorySeparatorChar.ToString());
                            tempconfigdir = configdir.Replace("/", Path.DirectorySeparatorChar.ToString());
                            string configdirname = tempconfigdir.Replace(Path.GetDirectoryName(configdir) + Path.DirectorySeparatorChar, "");
                            if(ListContainsString(Project.Properties["package.configs"], configdirname))
                            {
                                if(File.Exists(configdir + Path.DirectorySeparatorChar + "build" + Path.DirectorySeparatorChar + module + ".vcproj"))
                                    applicableConfigs = CombineStrings(applicableConfigs, configdirname);
                            }
                        }
                        if(applicableConfigs != "")
                        {
                            FileSet fs = new FileSet();
                            fs.Project = Project;
                            XmlElement fileset = _xmlDoc.CreateElement("fileset");
                            fileset.SetAttribute("name", module + "-input-vcproj-files");
                            foreach (string config in NantToVSTools.TrimAndSplit(applicableConfigs))
                            {
                                string vcprojfilename = Project.Properties["package." + eapkg.pkgName + ".builddir"] + Path.DirectorySeparatorChar + config + Path.DirectorySeparatorChar
                                    + "build" + Path.DirectorySeparatorChar + module + ".vcproj";
                                if (File.Exists(vcprojfilename))
                                {
                                    XmlElement includes = _xmlDoc.CreateElement("includes");
                                    includes.SetAttribute("name", vcprojfilename);
                                    fileset.AppendChild(includes);
                                }
                            }
                            fs.Initialize(fileset);

                            MergeMyVcproj mmvcproj = new MergeMyVcproj();
                            mmvcproj.Output = Project.Properties["package." + eapkg.pkgName + ".builddir"] + Path.DirectorySeparatorChar + module + ".vcproj";
                            mmvcproj.Project = Project;
                            mmvcproj.Inputs = fs;
                            mmvcproj.Execute();
                        }
                    }
                }
                else
                {
                    // merge vcproj for each group modules
                    foreach(string grp in eapkg.groupModules.Keys)
                    {
                        foreach(string mod in ((Hashtable) eapkg.groupModules[grp]).Keys)
                        {
                            FileSet fs = new FileSet();
                            fs.Project = Project;
                            XmlElement fileset = _xmlDoc.CreateElement("fileset");
                            fileset.SetAttribute("name", mod + "-input-vcproj-files");
                            string modulegroup = "";
                            if(grp != "runtime")
                                modulegroup = Path.DirectorySeparatorChar + grp;
                            foreach (string config in NantToVSTools.TrimAndSplit((string)((Hashtable)eapkg.groupModules[grp])[mod]))
                            {
                                string vcprojfilename = Project.Properties["package." + eapkg.pkgName + ".builddir." + config] + Path.DirectorySeparatorChar + config + Path.DirectorySeparatorChar
                                        + "build" + modulegroup + Path.DirectorySeparatorChar + mod + ".vcproj";
                                if (File.Exists(vcprojfilename))
                                {
                                    XmlElement includes = _xmlDoc.CreateElement("includes");
                                    includes.SetAttribute("name", vcprojfilename);
                                    fileset.AppendChild(includes);
                                }
                            }
                            fs.Initialize(fileset);

                            MergeMyVcproj mmvcproj = new MergeMyVcproj();
                            mmvcproj.Output = Project.Properties["package." + eapkg.pkgName + ".builddir"] + Path.DirectorySeparatorChar + mod + ".vcproj";
                            mmvcproj.Project = Project;
                            mmvcproj.Inputs = fs;
                            mmvcproj.Execute();
                        }
                    }
                }
            }
        }
        void MergeCsprojs()
        {
            Hashtable csproj2configs = new Hashtable();
            foreach (ManagedModule mm in _managedmodulelist)
            {
                if (mm.IsCSharp)
                {
                    if (csproj2configs.Contains(mm.VSProjLocation))
                        ((Hashtable)csproj2configs[mm.VSProjLocation])[mm.Config] = "";
                    else
                    {
                        Hashtable configs = new Hashtable();
                        configs[mm.Config] = "";
                        csproj2configs.Add(mm.VSProjLocation, configs);
                    }
                }
            }

            if (csproj2configs.Count != 0)
            {
                string currntConfig = Project.Properties["config"];
                foreach (string csprojlocation in csproj2configs.Keys)
                {
                    DoCsprojsMerge(csprojlocation, ((Hashtable) csproj2configs[csprojlocation]), currntConfig);
                    foreach (string config in ((Hashtable)csproj2configs[csprojlocation]).Keys)
                    {
                        string csconfigdir = Path.GetDirectoryName(csprojlocation) + Path.DirectorySeparatorChar + config;
                        string[] files = Directory.GetFiles(csconfigdir);
                        int numOfFiles = files.Length;
                        foreach (string file in files)
                        {
                            if(Path.GetFileNameWithoutExtension(file) == Path.GetFileNameWithoutExtension(csprojlocation))
                            {
                                File.Delete(file);
                                numOfFiles --;
                            }
                        }
                        if(numOfFiles == 0)
                            Directory.Delete(Path.GetDirectoryName(csprojlocation) + Path.DirectorySeparatorChar + config);
                    }
                }
            }
        }
        private void DoCsprojsMerge(string csprojlocation, Hashtable configs, string currentconfig)
        {
            XmlDocument csprojDoc = new XmlDocument();
            XmlDocument csprojDocUserFile = new XmlDocument();
            XmlDocument finalCSProj = new XmlDocument();
            XmlDocument finalCSProjUserFile = new XmlDocument();
            XmlNamespaceManager nsmgr = new XmlNamespaceManager(finalCSProj.NameTable);
            nsmgr.AddNamespace("msb", "http://schemas.microsoft.com/developer/msbuild/2003");
            string settingXPath = "/msb:Project/msb:PropertyGroup[msb:OutputPath]";
            string buildeventXPath = "/msb:Project/msb:PropertyGroup[msb:PreBuildEvent]";
            string referenceXPath = "/msb:Project/msb:ItemGroup[msb:Reference or msb:ProjectReference]";
            string comreferenceXPath = "/msb:Project/msb:ItemGroup[msb:COMReference ]";
            string filesXPath = "/msb:Project/msb:ItemGroup[msb:Compile or msb:Content or msb:EmbeddedResource]";

            if (configs.Contains(currentconfig))
            {
                string configCsproj = Path.GetDirectoryName(csprojlocation) + Path.DirectorySeparatorChar
                                    + currentconfig + Path.DirectorySeparatorChar
                                    + Path.GetFileName(csprojlocation);
                finalCSProj.Load(configCsproj);
                if (File.Exists(configCsproj + ".user"))
                    finalCSProjUserFile.Load(configCsproj + ".user");
            }

            foreach (string config in configs.Keys)
            {
                string configCsproj = Path.GetDirectoryName(csprojlocation) + Path.DirectorySeparatorChar
                                    + config + Path.DirectorySeparatorChar
                                    + Path.GetFileName(csprojlocation);
                if (! finalCSProj.HasChildNodes)
                {
                    finalCSProj.Load(configCsproj);
                    if (File.Exists(configCsproj + ".user"))
                        finalCSProjUserFile.Load(configCsproj + ".user");
                }
                else if(config != currentconfig)
                {
                    csprojDoc.Load(configCsproj);
                    if (!finalCSProjUserFile.HasChildNodes)
                    {
                        if (File.Exists(configCsproj + ".user"))
                            finalCSProjUserFile.Load(configCsproj + ".user");
                    }
                    else if (File.Exists(configCsproj + ".user"))
                    {
                        csprojDocUserFile.Load(configCsproj + ".user");
                        XmlNode configNode = csprojDocUserFile.DocumentElement.SelectSingleNode("/msb:Project/msb:PropertyGroup[1]", nsmgr);
                        if (configNode != null)
                            finalCSProjUserFile.DocumentElement.SelectSingleNode("/msb:Project", nsmgr).AppendChild(finalCSProjUserFile.ImportNode(configNode, true));
                    }

                    XmlNode ConfigSettingsNode = csprojDoc.DocumentElement.SelectSingleNode(settingXPath, nsmgr);
                    XmlNode BuildEventsNode = csprojDoc.DocumentElement.SelectSingleNode(buildeventXPath, nsmgr);
                    XmlNode ReferencesNode = csprojDoc.DocumentElement.SelectSingleNode(referenceXPath, nsmgr);
                    XmlNode ComReferencesNode = csprojDoc.DocumentElement.SelectSingleNode(comreferenceXPath, nsmgr);
                    XmlNode FilesNode = csprojDoc.DocumentElement.SelectSingleNode(filesXPath, nsmgr);
                    if (ConfigSettingsNode == null)
                        throw new BuildException("Error: missing configuration section in '" + configCsproj + "'!");
                    XmlNode lastConfigSettingNode = finalCSProj.DocumentElement.SelectSingleNode("/msb:Project/msb:PropertyGroup[msb:OutputPath][last()]", nsmgr);
                    if(lastConfigSettingNode != null)
                        finalCSProj.DocumentElement.SelectSingleNode("/msb:Project", nsmgr).InsertAfter(finalCSProj.ImportNode(ConfigSettingsNode, true), lastConfigSettingNode);
                    XmlNode lastReferenceNode = finalCSProj.DocumentElement.SelectSingleNode("/msb:Project/msb:ItemGroup[msb:Reference or msb:ProjectReference][last()]", nsmgr);
                    if(lastReferenceNode != null && ReferencesNode != null)
                        finalCSProj.DocumentElement.SelectSingleNode("/msb:Project", nsmgr).InsertAfter(finalCSProj.ImportNode(ReferencesNode, true), lastReferenceNode);
                    XmlNode lastComReferenceNode = finalCSProj.DocumentElement.SelectSingleNode("/msb:Project/msb:ItemGroup[msb:COMReference][last()]", nsmgr);
                    if (lastComReferenceNode != null && ReferencesNode != null)
                        finalCSProj.DocumentElement.SelectSingleNode("/msb:Project", nsmgr).InsertAfter(finalCSProj.ImportNode(ComReferencesNode, true), lastComReferenceNode);

                    XmlNode lastFileNode = finalCSProj.DocumentElement.SelectSingleNode("/msb:Project/msb:ItemGroup[msb:Compile or msb:Content or msb:EmbeddedResource][last()]", nsmgr);
                    if(lastFileNode != null && FilesNode != null)
                        finalCSProj.DocumentElement.SelectSingleNode("/msb:Project", nsmgr).InsertAfter(finalCSProj.ImportNode(FilesNode, true), lastFileNode);

                    if (null != BuildEventsNode && null != BuildEventsNode.Attributes.GetNamedItem("Condition"))
                    {
                        XmlNode lastBuildEventsNode = finalCSProj.DocumentElement.SelectSingleNode("/msb:Project/msb:PropertyGroup[msb:PreBuildEvent][last()]", nsmgr);
                        if (lastBuildEventsNode != null)
                            finalCSProj.DocumentElement.SelectSingleNode("/msb:Project", nsmgr).InsertAfter(finalCSProj.ImportNode(BuildEventsNode, true), lastBuildEventsNode);
                    }
                }
            }
            SaveCsProjIfUpdated(finalCSProj, csprojlocation); 
            
            if (finalCSProjUserFile.HasChildNodes)
                finalCSProjUserFile.Save(csprojlocation + ".user");
        }

        private void SaveCsProjIfUpdated(XmlDocument finalCSProj, string csprojlocation)
        {
            if (!File.Exists(csprojlocation))
            {
                finalCSProj.Save(csprojlocation);
                return;
            }
            // Compare file contents and save only if necessary:

            XmlDocument originalCSProj = new XmlDocument();
            if (File.Exists(csprojlocation + ".backup"))
            {
                originalCSProj.Load(csprojlocation + ".backup");
            }

            if (originalCSProj.DocumentElement == null 
                || finalCSProj.DocumentElement == null
                || NeedUpdate(finalCSProj.DocumentElement.ChildNodes, originalCSProj.DocumentElement.ChildNodes))
            {
                finalCSProj.Save(csprojlocation);

                //Log.WriteLineIf(Verbose, LogPrefix + "Updating csproj file '{0}'", csprojlocation);
            }
            else
            {
                File.Copy(csprojlocation + ".backup", csprojlocation, true);
                //Log.WriteLineIf(Verbose, LogPrefix + "Skipping update of csproj file '{0}'", csprojlocation);
            }
            if(File.Exists(csprojlocation + ".backup"))
            {
                File.Delete(csprojlocation + ".backup");
            }
        }

        private bool NeedUpdate(XmlNodeList newNodes, XmlNodeList origNodes)
        {
            if (newNodes == null)
            {
                return false;
            }
            if (origNodes == null)
            {
                return true;
            }
            if (newNodes.Count != origNodes.Count)
            {
                return true;
            }
            for (int i = 0; i < newNodes.Count; i++)
            {
                if (String.Equals(newNodes[i].Name, "ProductVersion", StringComparison.InvariantCultureIgnoreCase) &&
                   String.Equals(origNodes[i].Name, "ProductVersion", StringComparison.InvariantCultureIgnoreCase))
                {
                    newNodes[i].InnerXml = origNodes[i].InnerXml;
                }
                else if (String.Equals(newNodes[i].Name, "Compile", StringComparison.InvariantCultureIgnoreCase) &&
                         String.Equals(origNodes[i].Name, "Compile", StringComparison.InvariantCultureIgnoreCase))
                {
                    XmlNode origCodeNode = null;
                    XmlNode newCodeNode = null;
                    for(int j = 0; j < origNodes[i].ChildNodes.Count; j++)
                    {
                        if(origNodes[i].ChildNodes[j].Name == "SubType")
                        {
                            origCodeNode = origNodes[i].ChildNodes[j];
                        }
                    }
                    for(int j = 0; j < newNodes[i].ChildNodes.Count; j++)
                    {
                        if(newNodes[i].ChildNodes[j].Name == "SubType")
                        {
                            newCodeNode = newNodes[i].ChildNodes[j];
                        }
                    }
                    if (origCodeNode != null)
                    {
                        if (newCodeNode != null)
                        {
                            newCodeNode.InnerXml = origCodeNode.InnerXml;
                        }
                        else
                        {
                            newNodes[i].AppendChild(origCodeNode.CloneNode(true));
                        }
                    }
                    else
                    {
                        if (newCodeNode != null)
                        {
                            newNodes[i].RemoveChild(newCodeNode);
                        }
                    }
                }


                if(    0 != String.Compare(newNodes[i].Name, origNodes[i].Name, StringComparison.InvariantCultureIgnoreCase)
                    || 0 != String.Compare(newNodes[i].Value, origNodes[i].Value, StringComparison.InvariantCultureIgnoreCase)
                    )
                {
                    return true;
                }

                // Compare attributes
                if (newNodes[i].Attributes != null && origNodes[i].Attributes != null)
                {

                    if (newNodes[i].Attributes.Count != origNodes[i].Attributes.Count)
                    {
                        return true;
                    }

                    for (int k = 0; k < newNodes[i].Attributes.Count; k++)
                    {
                        if (0 != String.Compare(newNodes[i].Attributes[k].Name, origNodes[i].Attributes[k].Name, StringComparison.InvariantCultureIgnoreCase)
                            || 0 != String.Compare(newNodes[i].Attributes[k].Value, origNodes[i].Attributes[k].Value, StringComparison.InvariantCultureIgnoreCase)
                            )
                        {
                            return true;
                        }

                    }
                }
                else if(!(newNodes[i].Attributes == null && origNodes[i].Attributes == null))
                {
                    return true;
                }

                if(NeedUpdate(newNodes[i].ChildNodes, origNodes[i].ChildNodes))
                {
                    return true;
                }
                
            }
            return false;
        }

        void Vcproj(EAPackage eapkg, string cfg)
        {            
            // if nantProject is not null, it means that it needs to run generate-sln-all target instead
            // of the rwconfig-2.0 workflow.
            if(eapkg.slnTarget != null)
                return;
            // if the package is not autobuildable, then just return
            if(eapkg.autoBuildClean == false)
                return;

            if(eapkg.crossOnly == "true" && eapkg.nativeOnly == "true")
                throw new BuildException(eapkg.pkgName + ": rwconfig.native-only and rwconfig.cross-only can not both be set to true");

            string configs = cfg;
            if(cfg.IndexOf("cross") == -1) // if the current config is native
            {
                string crossconfig = eapkg.GetExploredCrossConfig(cfg);
                if(crossconfig == "")// if no explored cross config, then treat it as usual
                {
                    if(eapkg.crossOnly == "true")
                        return;
                    if(! ListContainsString(eapkg.nativeConfigs, cfg))
                        return;
                }
                else 
                {
                    if(eapkg.crossOnly == "true" || ! ListContainsString(eapkg.nativeConfigs, cfg))
                        configs = crossconfig;
                    else
                        configs = cfg + " " + crossconfig;
                }
            }

            if(eapkg.nativeOnly == "true" && cfg.IndexOf("cross") != -1)
                return;

            if(cfg.IndexOf("cross") != -1 && ! ListContainsString(eapkg.crossConfigs, cfg))
                return;

            string [] groups = _groups;
            if(Group != "allall")
            {
                if(PackageList != "" && (! ListContainsString(PackageList, eapkg.pkgName)))
                {
                    groups = new string [] {"runtime"};
                }
                else if(eapkg.pkgName != Project.Properties["package.name"] && PackageList == "")
                {
                    groups = new string [] {"runtime"};
                }
            }

            Hashtable groupModules = new Hashtable();
            foreach(string group in groups )
            {
                groupModules.Add(group, new Hashtable());
                if(! eapkg.groupModules.Contains(group))
                {
                    eapkg.groupModules.Add(group, new Hashtable());
                }
            }
            foreach(string group in groups )
            {
                Hashtable tempCurrModules = (Hashtable) groupModules[group];
                foreach(string config in NantToVSTools.TrimAndSplit(configs))
                {
                    if(config == string.Empty)
                        continue;
                    // generate vsprojs for all modules of each group for current config
                    Project proj = (Project) _projects[eapkg.pkgName + "_" + config];
                    if(proj == null)
                        continue;

                    foreach (string property in _properties.Keys)
                    {
                        string prop = property;
                        if (prop.EndsWith("." + cfg + ".vsproj"))
                        {
                            prop = prop.Replace("." + cfg + ".vsproj", "");
                            proj.Properties[prop] = (string)_properties[property];
                        }
                    }

                    string constrainedBuildModules = "";
                    if(eapkg.pkgName != Project.Properties["package.name"])
                        constrainedBuildModules = (string) _properties[eapkg.pkgName + "." + group + ".buildmodules." + config];
                    string buildModules = eapkg.pkgName;
                    if(group != "runtime")
                        buildModules = group + "-" + buildModules;
					string supressedBuildModules = ""; 
                    if(proj.Properties.Contains(group + ".buildmodules"))
                    {
                        if(constrainedBuildModules == "" || constrainedBuildModules == null)
                            buildModules = proj.Properties[group + ".buildmodules"];
                        else
                            buildModules = SortConstrainedBuildModules(constrainedBuildModules, proj.Properties[group + ".buildmodules"], ref supressedBuildModules);
                    }
                    else if(! proj.Properties.Contains(group + ".buildtype") && ! proj.Properties.Contains(group + ".builddependencies"))
                    {
                        // zero module
                        continue;
                    }

                    string tempModulesString = "";
                    ArrayList cSharpModules = new ArrayList();
                    ArrayList managedCPPModules = new ArrayList();
                    foreach(string module in NantToVSTools.TrimAndSplit(buildModules))
                    {
                        if(module != string.Empty)
                        {
                            string moduleKey = module;
                            if (group != "runtime" && moduleKey == group + "-" + eapkg.pkgName)
                            {
                                moduleKey = eapkg.pkgName;
                            }
                            ManagedModule mm = _managedmodulelist.Find(eapkg.pkgName + "*" + config + "*" + moduleKey + "*" + group);
                            string tempModule = module;
                            if(module == String.Empty)
                                continue;
                            // if it is a cross config, then we need to add cross- prefix to the module name
                            if(config.ToLower().IndexOf("cross") != -1)
                            {
                                if(! module.StartsWith("cross-"))
                                {
                                    proj.Properties.Add(module + ".vcproj-name-cross", "cross-" + module);
                                    _properties[eapkg.pkgName + "." + module + ".vcproj-name-cross." + config] = "cross-" + module;
                                    tempModule = "cross-" + module;
                                }
                            }
                            string buildtype = (string) proj.Properties[group + "." + module + ".buildtype"];
                            if(buildtype == null)
                                buildtype = (string) proj.Properties[group + ".buildtype"];
                            if(buildtype != null)
                            {
                                if (mm == null || (!mm.IsCSharp)) // not a CSharp module
                                {
                                    if (mm != null && (!mm.IsCSharp))
                                        managedCPPModules.Add(mm);
                                    Hashtable currModules = (Hashtable)eapkg.groupModules[group];
                                    // check duplicate module names in other groups
                                    foreach (string othergroup in groups)
                                    {
                                        if (othergroup != group)
                                        {
                                            if (((Hashtable)eapkg.groupModules[othergroup]).Contains(tempModule))
                                            {
                                                string errorString = String.Format("Error: Found duplicated module name {0} between groups {1} and {2} in package {3}", tempModule, group, othergroup, eapkg.pkgName);
                                                throw new BuildException(errorString);
                                            }
                                        }
                                    }
                                    // save each config for the module for the final merge step
                                    if (currModules.Contains(tempModule))
                                    {
                                        // if two configs are the same, it means this module+config already been generated before. this only applies to cross modules that have native module depend on it
                                        if (!ListContainsString(((string)currModules[tempModule]), config))
                                        {
                                            currModules[tempModule] = ((string)currModules[tempModule]) + " " + config;
                                            tempCurrModules[tempModule] = config;
                                            tempModulesString = tempModulesString + module + " ";
                                        }
                                    }
                                    else
                                    {
                                        currModules[tempModule] = config;
                                        tempCurrModules[tempModule] = config; // this is for merging only this config
                                        tempModulesString = tempModulesString + module + " ";
                                    }
                                    eapkg.groupModules[group] = currModules;
                                    groupModules[group] = tempCurrModules;
                                }
                                else // found CSharp module
                                {
                                    cSharpModules.Add(mm);
                                }
                            }
                        }
                    }

                    proj.Properties[PROPERTY_SKIP_BUILD_NANTTOVSTOOLS] = "true";
                    if (proj.Properties.Contains(group + ".buildmodules"))
                    {
                        proj.Properties[group + ".buildmodules.temp"] = proj.Properties[group + ".buildmodules"];
                    }
                    // Always check if there are any CSharp modules
                    if (cSharpModules.Count != 0)
                    {
                        //Copy project into backup location for slnmerger.
                        foreach (ManagedModule mmodule in cSharpModules) // remove projectreference optionsets
                        {
                            if (IsBuildingEasharpCSC(proj))
                                continue;

                            if (File.Exists(mmodule.VSProjLocation) && !File.Exists(mmodule.VSProjLocation + ".backup"))
                            {
                                File.Copy(mmodule.VSProjLocation, mmodule.VSProjLocation+".backup", true); // save it for merge step
                            }
                        }

                        string cSharpModulesString = "";
                        foreach (ManagedModule mmodule in cSharpModules) // add projectreference optionsets
                        {
                            cSharpModulesString = CombineStrings(cSharpModulesString, mmodule.Module);
                            if (mmodule.ProjRefCount != 0)
                            {
                                mmodule.SetProjRefOptionSet(proj); 
                            }
                        }
                        if (proj.Properties.Contains(group + ".buildmodules"))
                        {
                            proj.Properties[group + ".buildmodules"] = cSharpModulesString;
                        }
						proj.Properties[group + ".supressedBuildModules"] = supressedBuildModules;
						
                        if (group == "runtime")
                            ProjectTargetFind(proj, TARGET_CSPROJ).Copy().Execute();
                        else
                            ProjectTargetFind(proj, TARGET_CSPROJ + group).Copy().Execute();
                        if (proj.Properties.Contains(group + ".buildmodules"))
                        {
                            // restore group.buildmodules properties
                            proj.Properties[group + ".buildmodules"] = proj.Properties[group + ".buildmodules.temp"];
                        }
                        foreach (ManagedModule mmodule in cSharpModules) // remove projectreference optionsets
                        {
                            if (IsBuildingEasharpCSC(proj))
                                continue;
 
                            string configVsprojFileLocation = Path.GetDirectoryName(mmodule.VSProjLocation) + Path.DirectorySeparatorChar
                                + config + Path.DirectorySeparatorChar + Path.GetFileName(mmodule.VSProjLocation);
                            Directory.CreateDirectory(Path.GetDirectoryName(mmodule.VSProjLocation) + Path.DirectorySeparatorChar + config);
                            File.Copy(mmodule.VSProjLocation, configVsprojFileLocation, true); // save it for merge step
                            if (File.Exists(mmodule.VSProjLocation + ".user"))
                                File.Copy(mmodule.VSProjLocation + ".user", configVsprojFileLocation + ".user", true);
                            if (mmodule.ProjRefCount != 0)
                                mmodule.RemoveProjRefOptionSet(proj);
                        }
                        cSharpModules.Clear();
                    }

                    if(tempCurrModules.Count != 0) // make sure to only generate the same module+config once
                    {
                        foreach (ManagedModule mmodule in managedCPPModules) // add projectreference optionsets
                        {
                            if (mmodule.ProjRefCount != 0)
                            {
                                mmodule.SetProjRefOptionSet(proj);
                            }
                        }
                        if(proj.Properties.Contains(group + ".buildmodules"))
                        {
                            proj.Properties[group + ".buildmodules"] = tempModulesString;
                        }
                        proj.Properties["eaconfig.build.target"] = "eaconfig-vcproj";
                        proj.Properties["eaconfig.build.group"] = group;
                        proj.Targets.Find("eaconfig-build-caller").Copy().Execute();
                        if(proj.Properties.Contains(group + ".buildmodules"))
                        {
                            // restore group.buildmodules properties
                            proj.Properties[group + ".buildmodules"] = proj.Properties[group + ".buildmodules.temp"];
                        }
                        foreach (ManagedModule mmodule in managedCPPModules) // remove projectreference optionsets
                        {
                            if (mmodule.ProjRefCount != 0)
                            {
                                mmodule.RemoveProjRefOptionSet(proj);
                            }
                        }
                        managedCPPModules.Clear();
                    }
                    if (proj.Properties.Contains(group + ".buildmodules"))
                    {
                        proj.Properties.Remove(group + ".buildmodules.temp");
                    }
                    proj = null;
                }
            }
            if (IsBuildingEasharpCSC(Project))
            {
                return;
            }

            // if only generating for one config, we don't do the merge step but should store the vcprojs' locations
            // for slnmaker to use later on.
            if(Project.Properties.Contains("generate-single-config"))
            {
                foreach(string grp in groupModules.Keys)
                {
                    foreach(string mod in ((Hashtable) groupModules[grp]).Keys)
                    {
                        string groupDir = "";
                        if(grp != "runtime")
                            groupDir = Path.DirectorySeparatorChar + grp;     
                        string currentconfig = (string) ((Hashtable) groupModules[grp])[mod];   
                        string property = eapkg.pkgName + "." + mod + ".vcprojlocation." + currentconfig;
                        string propertyValue = Project.Properties["package." + eapkg.pkgName + ".builddir"] + 
                            Path.DirectorySeparatorChar + currentconfig + 
                            Path.DirectorySeparatorChar + "build" + groupDir +
                            Path.DirectorySeparatorChar + mod + ".vcproj";
                        _properties[property] = propertyValue;
                        // if we are in native config but need cross module, we need to store that cross module's vsprojlocation in native config as well so that 
                        // slnmaker can find it when generating sln for native configs.
                        if(currentconfig != cfg)
                            _properties[eapkg.pkgName + "." + mod + ".vcprojlocation." + cfg] = propertyValue;
                    }
                }
            }
            else
            {
                // We need to copy the vcproj file to the package.builddir where slnmaker looks for vcproj files.
                foreach(string grp in groupModules.Keys)
                {
                    foreach(string mod in ((Hashtable) groupModules[grp]).Keys)
                    {
                        string groupDir = "";
                        if(grp != "runtime")
                            groupDir = Path.DirectorySeparatorChar + grp;
                        string config = (string) ((Hashtable) groupModules[grp])[mod];
                        string vcprojfilelocation = Project.Properties["package." + eapkg.pkgName + ".builddir." + config] + 
                            Path.DirectorySeparatorChar + config + 
                            Path.DirectorySeparatorChar + "build" + groupDir +
                            Path.DirectorySeparatorChar + mod + ".vcproj";
                        string finalvcprojfilelocation = Project.Properties["package." + eapkg.pkgName + ".builddir." + config] + 
                            Path.DirectorySeparatorChar + mod + ".vcproj";

                        
                        // a hack for pc-vc-tools config

                        if (IsToolConfigBuild(Project.Properties["package.configs"]))
                        {							
                            if (config.IndexOf("vc-tool-") == -1)
                            {
                                continue;
                            }
                            string ext = Path.GetExtension(finalvcprojfilelocation);
                            if (ext == ".vcproj" || ext == ".csproj")
                            {
                                string fn = Path.GetFileNameWithoutExtension(finalvcprojfilelocation);
                                string dir = Path.GetDirectoryName(finalvcprojfilelocation);
                                finalvcprojfilelocation = Path.Combine(dir, fn + "-tools" + ext);
                            }
                        }
                        File.Copy(vcprojfilelocation, finalvcprojfilelocation, true);
                    }
                }
            }
            _packages[eapkg.pkgName] = eapkg;
        }

		public static bool IsToolConfigBuild(string configs)
		{
			int numTooConfigs = 0;
			string[] configList = NantToVSTools.TrimAndSplit(configs);
			foreach (string config in configList)
			{
				if (config.IndexOf("vc-tool-") != -1)
				{
					numTooConfigs++;
				}
			}
			return (numTooConfigs == configList.Length);
		}


        void SlnMaker(EAPackage eapkg, string cfg)
        {
            // if slnTarget is not null, it means that it needs to run generate-sln-all target instead
            // of the rwconfig-2.0 workflow. This is for backward compatibility with rwconfig 1.x
            if(eapkg.slnTarget != null)
                return;
            // if the package is not autobuildable, then just return
            if(eapkg.autoBuildClean == false)
                return;

            string configs = cfg;
            if(cfg.IndexOf("cross") == -1) // if the current config is native
            {
                string crossconfig = eapkg.GetExploredCrossConfig(cfg);
                if(crossconfig == "")
                {
                    if(eapkg.crossOnly == "true")
                        return;
                    if(! ListContainsString(eapkg.nativeConfigs, cfg))
                        return;
                }
                else
                {
                    // if found native-dependon-cross config, and package.configs doesn't contain this cross config
                    // then it means no more constrain-on-buildmodules gathering operation, we can make sln for this cross config now.
                    if(! ListContainsString(originalPackageConfigs, crossconfig))
                    {
                        if(eapkg.crossOnly == "true" || ! ListContainsString(eapkg.nativeConfigs, cfg))
                            configs = crossconfig;
                        else
                            configs = cfg + " " + crossconfig;
                    }
                    else // if package.configs contains this cross config, then we hold this cross config because there might be more buildmodule-constrains gathering
                    {
                        if(eapkg.crossOnly == "true" || ! ListContainsString(eapkg.nativeConfigs, cfg))
                            return;
                    }
                }
            }

            if(eapkg.nativeOnly == "true" && cfg.IndexOf("cross") != -1)
                return;

            if(cfg.IndexOf("cross") != -1 && ! ListContainsString(eapkg.crossConfigs, cfg))
                return;

            string [] groups = _groups;
            if(Group != "allall")
            {
                if(PackageList != "" && (! ListContainsString(PackageList, eapkg.pkgName)))
                {
                    groups = new string [] {"runtime"};
                }
                else if(eapkg.pkgName != Project.Properties["package.name"] && PackageList == "")
                {
                    groups = new string [] {"runtime"};
                }
            }

            foreach(string config in NantToVSTools.TrimAndSplit(configs))
            {
                if(config == string.Empty)
                    continue;

                Project proj = (Project) _projects[eapkg.pkgName + "_" + config];
                if(proj == null && config.IndexOf("cross") != -1) // the sln for this cross config has already been generated.
                    continue;
                string packageBuilddir = proj.Properties["package.builddir"];
                foreach(string grp in groups)
                {
                    proj.Properties["package.nantToVSTools.slnmaker.default.output"] = packageBuilddir + "\\" + config + "\\" + eapkg.pkgName + "_" + grp + ".sln";
                    // Get all initialize.xml properties for all relevant packages and all other relevant properties for slnmaking.
                    foreach(object property in _properties.Keys)
                    {
                        string prop = (string) property;
                        if(prop.EndsWith("." + config))
                        {
                            prop = prop.Replace("." + config, "");
                            proj.Properties[prop] = (string) _properties[property];
                        }
                    }
                    if(Project.Properties.Contains("generate-single-config"))
                    {
                        proj.Properties["generate-single-config"] = "true";
                    }
                    ProjectTargetFind(proj, "sln" + grp + "-slnmaker").Copy().Execute();
                    AddSlnToFileSets(eapkg.pkgName, grp, packageBuilddir + "\\" + config + "\\" + eapkg.pkgName + "_" + grp + ".sln");
                }
                _projects[eapkg.pkgName + "_" + config] = null;
                if(eapkg.pkgName != Project.Properties["package.name"] || (eapkg.pkgName == Project.Properties["package.name"] && config != Project.Properties["config"]))
                {
                    proj.Dispose();
                    proj = null;
                }
            }
        }  
        
        void InitializeGlobalVariables()
        {
            if(Project.Properties.Contains("slngenerator.generateslnforallpackages"))
            {
                if(Project.Properties["slngenerator.generateslnforallpackages"] == "true")
                    generateCompleteSlnsForAllPackages = true;
            }

            if(Group == "all" || Group == "allall")
            {
                _groups = new String [] {"runtime", "test", "example", "tool"};
            }
            else if(Group != "runtime")
            {
                _groups = new String [] {"runtime", Group};
            }
            else
            {
                _groups = new String [] {"runtime"};
            }
            
            if(Project.Properties.Contains("package.builddependencies.crossonly"))
            {
                _packagesCrossOnly = Project.Properties["package.builddependencies.crossonly"];
            }
            if(Project.Properties.Contains("package.builddependencies.nativeonly"))
            {
                _packagesNativeOnly = Project.Properties["package.builddependencies.nativeonly"];
            }

            string conflictedPackages = "";
            foreach(string cpackage in NantToVSTools.TrimAndSplit(_packagesCrossOnly))
            {
                if(cpackage != string.Empty)
                {
                    if(ListContainsString(_packagesNativeOnly, cpackage))
                    {
                        conflictedPackages = CombineStrings(conflictedPackages, cpackage);
                    }
                }
            }
            if(conflictedPackages != "")
                throw new BuildException("Can not set both cross-only and native-only to be true for build dependents '" + conflictedPackages + "'!");

            _targetElement = _xmlDoc.CreateElement("target");
            _targetElement.SetAttribute("name", "__private_do_dependent_task");
        }

        bool IsPackageCompatibleWithSlnGenerator()
        {
            if(! Project.Properties.Contains("rwconfig.enableproperties"))
            {
                // it's eaconfig package, so it's compatible with slngenerator
                return true;
            }
            else if(Project.Properties["rwconfig.enableproperties"].ToLower() == "false")
            {
                if (Project.Properties["config"].IndexOf("cross") != -1 || Project.Properties["package.configs"].IndexOf("cross") != -1)
                    throw new BuildException("Error: '" + Project.Properties["package.name"] + "' is an eaconfig package, and is not supposed to be built in cross configurations");

                // it's a drop-in replacement for eaconfig
                return true;
            }

            if(Project.Properties.Contains("rwconfig.enableslngeneration"))
            {
                if(Project.Properties["rwconfig.enableslngeneration"].ToLower() == "true")
                {
                    return true;
                }
            }
            return false;
        }
        void ValidatePackageConfigs()
        {
            if(Project.Properties.Contains("rwconfig.cross-only"))
            {
                if(Project.Properties["rwconfig.cross-only"] == "true")
                {
                    //check if package.configs contains any native configs
                    foreach(string config in NantToVSTools.TrimAndSplit(Project.Properties["package.configs"]))
                    {
                        if(config != string.Empty)
                        {
                            if(config.IndexOf("cross") == -1)
                            {
                                throw new BuildException("ERROR: Can not have native configs listed in 'package.configs' for cross-only package '" + Project.Properties["package.name"] + "'!");
                            }
                        }
                    }
                }
            }
        }
        // Reorder the package.configs to be "native1 cross1 native2 cross2 ... nativeN crossN" for the sake of performance and accuracy
        string ReOrderPackageConfigs()
        {
            Hashtable nativeConfigs = new Hashtable();
            string crossConfigs = "";
            char [] delim = {'-'};
            string pkgConfigs = "";
            foreach(string config in NantToVSTools.TrimAndSplit(Project.Properties["package.configs"]))
            {
                if(config == string.Empty)
                    continue;
                if(! ListContainsString(pkgConfigs, config))
                    pkgConfigs = pkgConfigs + " " + config;
                else
                    Console.WriteLine("\nWARNING: config '" + config + "' is defined multiple times in property 'package.configs'!\n" );
            }
            foreach(string config in NantToVSTools.TrimAndSplit(pkgConfigs))
            {
                if(config == string.Empty)
                    continue;
                if(config.IndexOf("cross") == -1)
                {
                    string [] configComponents = config.Split(delim);
                    string configset = "";
                    for(int i=2; i<configComponents.Length; i++)
                    {
                        if(i == (configComponents.Length -1))
                            configset = configset + configComponents[i];
                        else
                            configset = configset + configComponents[i] + "-";
                    }
                    string correspondingCrossConfig = "pc-vc-cross-" + configComponents[0] + "-" + configset;
                    nativeConfigs[config] = correspondingCrossConfig;
                }                    
                else
                {
                    crossConfigs = crossConfigs + " " + config;
                }
            }
            if(nativeConfigs.Count != 0 && crossConfigs != "")
            {
                string packageConfigs = "";
                foreach(string nconfig in nativeConfigs.Keys)
                {
                    packageConfigs = packageConfigs + nconfig + " ";
                    string cconfig = (string) nativeConfigs[nconfig];
                    if(ListContainsString(crossConfigs, cconfig))
                        packageConfigs = packageConfigs + cconfig + " ";
                }
                foreach(string cconfig in NantToVSTools.TrimAndSplit(crossConfigs))
                {
                    if(cconfig == string.Empty)
                        continue;
                    if(! ListContainsString(packageConfigs, cconfig))
                        packageConfigs = packageConfigs + cconfig + " ";
                }
                return packageConfigs;
            }
            return pkgConfigs;
        }

        protected override void ExecuteTask()
        {
            if(! IsPackageCompatibleWithSlnGenerator())
            {
                throw new BuildException("ERROR: This package does not support automated SLN generation. Please call 'generate-sln' or set the property 'rwconfig.enableslngeneration' to true !");
            }      
      
            ValidatePackageConfigs();

            System.DateTime dt1 = System.DateTime.Now;

            string packageConfigs = ReOrderPackageConfigs();
            string packageBuildDir = Project.Properties["package.builddir"];
            string packageName = Project.Properties["package.name"];
            originalPackageConfigs = Project.Properties["package.configs"];
            InitializeGlobalVariables();

            if(Project.Properties.Contains("generate-single-config"))
            {
                packageConfigs = Project.Properties["config"];
                Project.Properties.UpdateReadOnly("package.configs", packageConfigs);
            }

            foreach(string config in NantToVSTools.TrimAndSplit(packageConfigs))
            {
                if(config == string.Empty)
                    continue;

                if(PackageList != "")
                {
                    foreach(string pkgname in NantToVSTools.TrimAndSplit(PackageList))
                    {
                        AddPackageProperties(pkgname);
                        FindBuildDependencies(pkgname, config, packageConfigs, Group, true, false, null);
                    }
                }
                else
                {
                    FindBuildDependencies(packageName, config, packageConfigs, Group, true, false, null);
                }

                // instantiate one fileset for each package (input to slnmerger)
                if(generateCompleteSlnsForAllPackages || PackageList != "")
                {
                    foreach(object package in _packages.Keys)
                    {
                        EAPackage eap = (EAPackage) _packages[package];
                        if (!_filesets.Contains(eap.pkgName))
                        {
                            XmlElement filesetElement = _xmlDoc.CreateElement("fileset");
                            filesetElement.SetAttribute("name", "sln_fileset_" + eap.pkgName);
                            _filesets[eap.pkgName] = filesetElement;
                        }
                    }
                }
                else
                {
                    if (!_filesets.Contains(packageName))
                    {
                        XmlElement filesetElement = _xmlDoc.CreateElement("fileset");
                        filesetElement.SetAttribute("name", "sln_fileset_" + packageName);
                        _filesets[packageName] = filesetElement;
                    }
                }

                // first execuate all packages using old rwconfigs
                foreach(object package in _packages.Keys)
                {
                    EAPackage eap = (EAPackage) _packages[package];
                    if(eap.slnTarget != null)
                    {
                        Project proj = (Project) _projects[eap.pkgName + "_" + config];
                        if(proj == null)
                            continue;
                        proj.Properties["package.nantToVSTools.slnmaker.default.output"] = proj.Properties["package.builddir"] + Path.DirectorySeparatorChar
                            + proj.Properties["config"] + Path.DirectorySeparatorChar
                            + eap.pkgName + ".sln";
                        if(ProjectTargetFind(proj, eap.slnTarget) != null)
                        {
                            ProjectTargetFind(proj, eap.slnTarget).Copy().Execute();
                            AddSlnToFileSets(eap.pkgName, "runtime", proj.Properties["package.builddir"] + Path.DirectorySeparatorChar
                                + proj.Properties["config"] + Path.DirectorySeparatorChar
                                + eap.pkgName + ".sln");
                        }
                        else
                        {
                            throw new BuildException(eap.pkgName + ": target " + eap.slnTarget + " is missing!");
                        }
                        _projects[eap.pkgName + "_" + config] = null;
                        proj.Dispose();
                        proj = null;
                        // This is to tell slnmaker to treat rwconfig1.x packages differently because
                        // the generated vcproj files break the one-config-at-a-time SLN generation semantics
                        _properties[eap.pkgName + ".is-using-rwconfig1x." + config] = "";
                    }
                }

                PopulateProjectReferences(config);

                /// START: TO BE REMOVED for backwards compatibility with customized slngroup-slnmaker.
                PopulateNativeToCrossBuildModuleConstrains(config);
                PopulateModuleDependencies();
                /// END: TO BE REMOVED

                // Generate vsproj and solution files for all slnmerger workflow packages
                Hashtable temppackages = (Hashtable) _packages.Clone();
                foreach(object package in temppackages.Keys)
                {       
                    Vcproj((EAPackage) temppackages[package], config);
                } 
                temppackages.Clear();

                // In case I'm building EASharp using recursive nant call I need only create csproject, but not solutions
                if (!Project.Properties.Contains("easharp.runtime.build.buildcsc"))
                {
                    foreach (object package in _packages.Keys)
                    {
                        SlnMaker((EAPackage)_packages[package], config);
                    }
                }
            }
            // In case I'm building EASharp using recursive nant call I'm done
            if (Project.Properties.Contains("easharp.runtime.build.buildcsc"))
                return;

            if (!Project.Properties.Contains("generate-single-config"))
            {
                MergeVcprojs();
                MergeCsprojs();
            }

            if(PackageList != "")
            {
                XmlElement megaslnfileset = _xmlDoc.CreateElement("fileset");
                megaslnfileset.SetAttribute("name", "megasolution");
                foreach(string pkgname in NantToVSTools.TrimAndSplit(PackageList))
                {
                    if(((XmlElement)_filesets[pkgname]).ChildNodes.Count != 0)
                    {
                        string output = MergeSolutions(pkgname, Project.Properties["package." + pkgname + ".builddir"], true);
                        XmlElement includes = _xmlDoc.CreateElement("includes");
                        includes.SetAttribute("name", output);
                        megaslnfileset.AppendChild(includes);
                    }
                }
                if(generateCompleteSlnsForAllPackages)
                {
                    foreach(object package in _packages.Keys)
                    {
                        EAPackage eap = (EAPackage) _packages[package];
                        if(! ListContainsString(PackageList, eap.pkgName))
                        {
                            string output = MergeSolutions(eap.pkgName, Project.Properties["package." + eap.pkgName + ".builddir"], false);
                            XmlElement includes = _xmlDoc.CreateElement("includes");
                            includes.SetAttribute("name", output);
                            megaslnfileset.AppendChild(includes);
                        }
                    }
                }
                SlnMerger slnMergerMegaSln = new SlnMerger(Project);
                FileSet fs = new FileSet();
                fs.Project = Project;
                fs.Initialize(megaslnfileset);
                slnMergerMegaSln.Inputs = fs;
                slnMergerMegaSln.Output = Project.Properties["package.builddir"] + Path.DirectorySeparatorChar + packageName + ".sln";
                if(! Directory.Exists(Project.Properties["package.builddir"]))
                    Directory.CreateDirectory(Project.Properties["package.builddir"]);
                slnMergerMegaSln.ExecuteSlnMergerTask();
            }
            else
            {
                // Merge all generated solution files to the final solution file.
                if (_filesets.Contains(packageName) && ((XmlElement)_filesets[packageName]).ChildNodes.Count != 0)
                {
                    MergeSolutions(packageName, packageBuildDir, true);
                }

                if(generateCompleteSlnsForAllPackages)
                {
                    foreach(object package in _packages.Keys)
                    {
                        EAPackage eap = (EAPackage) _packages[package];
                        if(eap.pkgName != packageName)
                        {
                            MergeSolutions(eap.pkgName, Project.Properties["package." + eap.pkgName + ".builddir"], false);
                        }
                    }
                }
            }

            // restore the package.configs
            Project.Properties.UpdateReadOnly("package.configs", originalPackageConfigs);

            System.DateTime dt2 = System.DateTime.Now;
            System.TimeSpan dt3 = dt2.Subtract(dt1);
            Console.WriteLine("start time:" + dt1.ToString());
            Console.WriteLine("end time:" + dt2.ToString());
            Console.WriteLine("time elapsed: " + dt3.TotalMilliseconds + " milliseconds");
        }
    }
}
