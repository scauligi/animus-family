﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Serialization;

namespace AnimusBuilder
{
    class MdCore
    {
        public static void Serialize<T>(T obj, string path)
        {

            StreamWriter sw = new StreamWriter(path, false);
            XmlSerializer ser = new XmlSerializer(typeof(T));
            ser.Serialize(sw, obj);
            sw.Close();
        }

        public static T Deserialize<T>(string path)
        {
            StreamReader sr = new StreamReader(path);
            XmlSerializer ser = new XmlSerializer(typeof(T));
            T output = (T)ser.Deserialize(sr);

            sr.Close();
            return output;

        }

        public static bool SetFileAttribute(string file, string attr, string val)
        {
            bool output = false;

            if (!File.Exists(file))
            {
                return output;
            }

            List<string> lines = File.ReadAllLines(file).ToList();

            for (int i = 0; i < lines.Count; i++)
            {
                if (lines[i].Contains("#define " + attr))
                {
                    lines[i] = "#define " + attr + " " + val;
                    output = true;
                    break;
                }
            }

            File.WriteAllLines(file, lines);
            return output;
        }

        public static bool EnsureMethodExists(string file, string method)
        {
            bool output = false;

            if (!File.Exists(file))
            {
                return output;
            }

            List<string> lines = File.ReadAllLines(file).ToList();
            var insertionPoint = lines.Count;

            for (int i = 0; i < lines.Count; i++)
            {
                if (lines[i].Contains(method))
                {
                    output = true;
                    break;
                }
                else if (lines[i].Contains("#undef mod_modname") || lines[i].Contains("#undef modMethod"))
                {
                    insertionPoint = Math.Min(i, insertionPoint);
                }
            }
            if (!output)
            {
                lines.Insert(insertionPoint, method + " { }");
                output = true;
            }

            File.WriteAllLines(file, lines);
            return output;
        }

        public static string BuildAnimus(ClBuildProfile bp, ClController controller, string animusPath, string modPath, string outputPath)
        {
            string output = "";

            if (animusPath.EndsWith(MdConstant.pseparator.ToString()))
            {
                //donothing
            }
            else
            {
                animusPath = animusPath + MdConstant.pseparator;
            }

            if (modPath.EndsWith(MdConstant.pseparator.ToString()))
            {
                //donothing
            }
            else
            {
                modPath = modPath + MdConstant.pseparator;
            }

            if (outputPath.EndsWith(MdConstant.pseparator.ToString()))
            {
                //donothing
            }
            else
            {
                outputPath = outputPath + MdConstant.pseparator;
            }

            if (bp.name.Contains("\"") || bp.name.Contains("\\") ||
                bp.driver_build.Contains("\"") || bp.driver_build.Contains("\\") ||
                bp.variant.Contains("\"") || bp.variant.Contains("\\"))
            {
                output = "Error: name, build, or variant contains illegal characters.";
                return output;
            }

            if (bp.hpins.Count < 1 || bp.vpins.Count < 1)
            {
                output = "Error: you need at least 1 pin per column and row.";
                return output;
            }

            if (!File.Exists(animusPath + MdConstant.pseparator + "animus.ino"))
            {
                output = "Error: animus.ino missing, animus path is invalid";
                return output;
            }

            if (!Directory.Exists(modPath))
            {
                output = "Error: mod folder missing, mod path is invalid.";
                return output;
            }

            var cmr = bp.CheckMods(controller, modPath);


            if (cmr == ClBuildProfile.CheckModResponse.DirectoryDoesNotExist)
            {
                output = "Error: mod directory does not exist.";
                return output;

            }
            else if (cmr == ClBuildProfile.CheckModResponse.ModDoesNotExist)
            {
                output = "Error: mod not found.";
                return output;
            }
            else if (cmr == ClBuildProfile.CheckModResponse.ModNamingError)
            {

                output = "Error: mod naming error, mod file name and mod name does not match.";
                return output;
            }
            else if (cmr == ClBuildProfile.CheckModResponse.PinConflictError)
            {

                output = "Error: Pin conflict error, a pin is used more than once.";
                return output;
            }
            else if (cmr == ClBuildProfile.CheckModResponse.PinDoesNotExist)
            {

                output = "Error: Pin does not exist for the selected controller.";
                return output;
            }
            else if (cmr == ClBuildProfile.CheckModResponse.EEPROMConflictError)
            {
                output = "Error: EEPROM conflict.";
                return output;
            }
            else if (cmr == ClBuildProfile.CheckModResponse.AllClear)
            {
                // vars
                string ofolder = outputPath + bp.bp_name + MdConstant.pseparator + Path.GetFileNameWithoutExtension(animusPath) + MdConstant.pseparator + "animus";
                Console.WriteLine(ofolder);
                string mafile = ofolder + MdConstant.pseparator + "animus.ino";
                string mmfile = ofolder + MdConstant.pseparator + "mod.ino";

                // copy main animus files
                MdCore.DirectoryCopy(animusPath, ofolder);

                // edit main animus file
                MdCore.SetFileAttribute(mafile, MdConstant.bRow, bp.vpins.Count.ToString());
                MdCore.SetFileAttribute(mafile, MdConstant.bCol, bp.hpins.Count.ToString());

                MdCore.SetFileAttribute(mafile, MdConstant.bName, "\"" + bp.name + "\"");

                MdCore.SetFileAttribute(mafile, MdConstant.bVariant, "\"" + bp.variant + "\"");

                MdCore.SetFileAttribute(mafile, MdConstant.bBuild, "\"" + bp.driver_build + "\"");

                MdCore.SetFileAttribute(mafile, MdConstant.bVPins, ListStringToString(bp.vpins));

                MdCore.SetFileAttribute(mafile, MdConstant.bHPins, ListStringToString(bp.hpins));

                MdCore.SetFileAttribute(mafile, MdConstant.bRefresh, bp.refresh);

                // edit main mod file

                MdCore.SetFileAttribute(mmfile, MdConstant.bMStart, GenerateModMethodString(bp.mods, MdConstant.bMEStartup));
                MdCore.SetFileAttribute(mmfile, MdConstant.bMPreCoord, GenerateModMethodString(bp.mods, MdConstant.bMEPreCoord));
                MdCore.SetFileAttribute(mmfile, MdConstant.bMPrePress, GenerateModMethodString(bp.mods, MdConstant.bMEPrePress));
                MdCore.SetFileAttribute(mmfile, MdConstant.bMKDown, GenerateModMethodString(bp.mods, MdConstant.bMEKDown));
                MdCore.SetFileAttribute(mmfile, MdConstant.bMKUp, GenerateModMethodString(bp.mods, MdConstant.bMEKUp));
                MdCore.SetFileAttribute(mmfile, MdConstant.bMLoop, GenerateModMethodString(bp.mods, MdConstant.bMELoop));
                MdCore.SetFileAttribute(mmfile, MdConstant.bMSerial, GenerateModMethodString(bp.mods, MdConstant.bMESerial));

                // copy mod files
                List<string> modpaths = bp.mods.Select(str => modPath + str).ToList();
                foreach(string str in modpaths)
                {
                    DirectoryCopy(str, ofolder, false);
                    string modName = str.Substring(modPath.Length);
                    string modFile = ofolder + MdConstant.pseparator + "mod_" + modName + ".ino";
                    EnsureMethodExists(modFile, MdConstant.bMFPreCoord);
                    EnsureMethodExists(modFile, MdConstant.bMFPrePress);
                }

            }
            else
            {

                output = "Error: Unknown build error.";
                return output;
            }

            /*
            List<string> modpool = new List<string>();
            modpool = Directory.GetDirectories(MdSetting.setting.modPath).Select(str => Path.GetFileNameWithoutExtension(str)).ToList();
            foreach (string mod in bp.mods)
            {
                if (modpool.Contains(mod))
                {
                    
                }
                else
                {
                    output = "Error: mod " + mod + " missing from the mod path.";
                    return output;
                }
            }
            */

            output = "Build Completed for " + bp.name;
            return output;
        }

        #region string operations
        public static string ListStringToString(List<string> input)
        {
            string output = "";
            foreach (string str in input)
            {
                output = output + str + ", ";
            }
            if (output.EndsWith(", "))
            {
                output = output.Substring(0, output.Length - 2);
            }
            return output;
        }


        public static string GenerateModMethodString(List<string> mods, string method)
        {
            string output = "";
            foreach(string mod in mods)
            {
                output = output + mod + method + " ";
            }
            if (output.EndsWith(" "))
            {
                output = output.Substring(0, output.Length - 1);
            }
            return output;

        }
        #endregion

        #region file io stuff
        public static void DirectoryCopy(string sourceFolder, string destinationFolder, bool deleteOldContent = true)
        {
            if (sourceFolder.EndsWith(MdConstant.pseparator.ToString()))
            {
                // donothing
            }
            else
            {
                sourceFolder = sourceFolder + MdConstant.pseparator;
            }

            if (destinationFolder.EndsWith(MdConstant.pseparator.ToString()))
            {
                // donothing
            }
            else
            {
                destinationFolder = destinationFolder + MdConstant.pseparator;
            }

            if (deleteOldContent)
            {
                DirectoryDelete(destinationFolder);
            }

            Directory.CreateDirectory(destinationFolder);

            if (Directory.Exists(sourceFolder))
            {
                foreach(string str in Directory.GetDirectories(sourceFolder, "*" ,SearchOption.AllDirectories))
                {
                    string temp = str.Replace(sourceFolder, destinationFolder);
                    Directory.CreateDirectory(temp);
                }
                foreach(string str in Directory.GetFiles(sourceFolder, "*", SearchOption.AllDirectories))
                {
                    string temp = str.Replace(sourceFolder, destinationFolder);
                    File.Copy(str, temp);
                }
            }
        }

        public static void DirectoryDelete(string target_dir)
        {
            if (Directory.Exists(target_dir))
            {

                string[] files = Directory.GetFiles(target_dir);
                string[] dirs = Directory.GetDirectories(target_dir);

                foreach (string file in files)
                {
                    File.SetAttributes(file, FileAttributes.Normal);
                    File.Delete(file);
                }

                foreach (string dir in dirs)
                {
                    DirectoryDelete(dir);
                }

                Directory.Delete(target_dir, false);
            }
        }

        #endregion
    }
}
