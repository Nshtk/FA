using System;
using System.Collections.Generic;
using System.IO;

namespace Practice_4
{
    class Program
    {
        class Explorer : IDisposable
        {
            private List<FileInfo> number_names = new List<FileInfo>();
            private Dictionary<FileInfo, string[]> buf = new Dictionary<FileInfo, string[]>();

            public bool GetFiles(string path, Predicate<string> isValidFile, Predicate<string> isValidContent) // Файлы распределяются по коллекциям: файлы с
                                                                                                               // нужными записями - в словарь, файлы с цифровыми
                                                                                                               // (или подходящими под другой критерий) именами (билеты) - в массив.
            {
                DirectoryInfo dir = new DirectoryInfo(path);
                if (!dir.Exists)
                {
                    Console.WriteLine("Directory doesnt exists.");
                    return false;
                }
                else
                {
                    FileInfo[] all_files = dir.GetFiles("*", SearchOption.AllDirectories);
                    List<FileInfo> specific_files = new List<FileInfo>();

                    foreach (var file in all_files)
                        if (isValidFile(file.Name))
                            specific_files.Add(file);
                    
                        
                    if (isValidContent != null)
                    {
                        foreach (var file in specific_files)
                        {
                            using (var reader = new StreamReader(file.FullName))
                            {
                                string content = reader.ReadToEnd();
                                if (isValidContent(content))
                                    buf.Add(file, content.Split('\n'));
                            }
                        }
                        if (number_names.Count == 0)
                        {
                            Console.WriteLine("No matching content files found.");
                            return false;
                        }
                    }
                    else
                    {
                        foreach (var file in specific_files)
                            number_names.Add(file);
                        if (number_names.Count == 0)
                        {
                            Console.WriteLine("No matching files with numbered names found.");
                            return false;
                        }
                    }

                    return true;
                }
            }
            public bool CreateOutput(string path)                               // Создается новое имя для pdf (или другого файла)
            {
                if (number_names.Count == 0 || buf.Count == 0)
                {
                    Console.WriteLine("No files to work with.");
                    return false;
                }
                    
                if (!Directory.Exists(path))
                    Directory.CreateDirectory(path);

                FileInfo current_path = new FileInfo(path);
                int i = 0;

                foreach(var file in buf.Keys)
                {
                    for(int j=0; j<buf[file].Length && i<number_names.Count; j++, i++)
                    {
                        string new_name = path + CreateName(file.Name, buf[file][j], 0);
                        string extension = GetExtension(number_names[i].Name);
                        uint number = 0;
                        if (File.Exists(new_name + extension))
                        {
                            new_name = path + CreateName(file.Name, buf[file][j], 1);
                            if (File.Exists(new_name + extension))
                            {
                                number++;
                                for (; File.Exists(new_name + " (" + number.ToString() + ')' + extension); number++);
                            }
                        }
                            
                        if(number==0)
                            number_names[i].CopyTo(new_name + extension);
                        else
                            number_names[i].CopyTo(new_name + " (" + number.ToString() + ')' + extension);
                    }
                    if (i == number_names.Count)
                        break;
                }
               
                return true;
            }

            private string CreateName(string name, string content, int mode)
            {
                string new_name;
                char[] content_ar = content.ToCharArray();
                List<List<char>> initials = new List<List<char>>(2); initials.Add(new List<char>()); initials.Add(new List<char>());

                List<char> second_name = new List<char>();
                for (int i = 0, j = 0; i < content_ar.Length; i++)
                {
                    if(mode==0)
                        initials[j].Add(content_ar[i]);
                    else
                        for(int i_plus_three=i+3; i<i_plus_three && content_ar[i] != ' '; i++)
                            initials[j].Add(content_ar[i]);
                    j++;
                    while (i < content_ar.Length - 1 && content_ar[i] != ' ')
                    {
                        i++;
                        if (i == content_ar.Length)
                            break;
                    }
                        
                    if(j==1)
                        for (++i; content_ar[i] != ' '; i++)
                            second_name.Add(content_ar[i]);
                }
                int index = name.IndexOf('.');
                if (index > 0)
                    name = name.Substring(0, index);
                
                new_name = name + '_' + new string(second_name.ToArray())+' '+ new string(initials[0].ToArray())+' '+ new string(initials[1].ToArray());

                return new_name;
            }
            private string GetExtension(string extension)
            {
                extension = extension.Substring(Math.Max(0, extension.Length - 4));
                return extension;
            }
            public void Dispose()
            {}
        }

        static Predicate<string> isNumber = delegate (string name)       // Могут быть применены любые правила проверки имени и содержимого файла с помощью предикатов
        {
            char[] arr = name.ToCharArray();
            for (int i = 1; i < name.Length && arr[i] != '.'; i++)
                if (!Char.IsDigit(arr[i]))
                    return false;
            return true;
        };
        static Predicate<string> isGroup = delegate (string name)
        {
            char[] arr = name.ToCharArray();
            if(arr[0]!='0')
                return false;
            for (int i = 1; i < name.Length && arr[i] != '.'; i++)
                if (!Char.IsDigit(arr[i]))
                    return false;
            return true;
        };
        static Predicate<string> isStudent = delegate (string content)
        {
            int spaces_count = 0;
            char[] arr = content.ToCharArray();

            for (int i = 0; i < content.Length; i++)
                if (arr[i]==' ')
                    spaces_count++;

            if(spaces_count>1)
                return true;
            return false;
        };
        static void Main(string[] args)
        {
            using (var Explorer = new Explorer())
            {
                Explorer.GetFiles(args[1], isNumber, null);
                Explorer.GetFiles(args[2], isGroup, isStudent);
                Explorer.CreateOutput(args[3]);
            }
        }
    }
}
