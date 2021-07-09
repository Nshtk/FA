using System;
using System.IO;

namespace Practice_Cs2
{
    class Program
    {
        enum severity
        {
            TRACE,
            DEBUG,
            INFORMATION,
            WARNING,
            ERROR,
            CRITICAL
        }
        class Logger : IDisposable
        {
            private StreamWriter writer;
            public Logger(string path)
            {
                writer = new StreamWriter(path);
            }

            public void Log(string text, severity level)
            {
                writer.WriteLine($"[{DateTime.Now:U}][{level}]: {text}");
            }
            public void Dispose()
            {
                writer.Dispose();
            }
        }
        static void Main(string[] args)
        {
            try
            {
                using (var logger = new Logger("output_file.txt"))          // Преобразуется в блок try{} finally{}, который позволяет вызвать 
                                                                            // Dispose() после обработки исключений; вызов дестркутора с Dispose() - плохо,
                                                                            // т.к. появляется проблема с повторным вызовом Dispose(), но ее можно решить путем
                                                                            // запрета вызова деструктора после явного вызова Dispose() либо (худший вариант) вызывать
                                                                            // деструктор с проверкой disposable объекта на null, зануляя этот объект после явного вызова Dispose().
                {
                    logger.Log("Some Stuff.", severity.INFORMATION);
                }
            }
            catch(FileNotFoundException e)
            {
                Console.WriteLine("File not found");
            }
        }
    }
}
