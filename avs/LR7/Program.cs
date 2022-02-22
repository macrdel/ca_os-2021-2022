using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace LR7
{
    class Program
    {
        static void Main(string[] args)
        {
            // Create two different encodings 21866 - koi-8u
            Encoding ascii = Encoding.ASCII;
            Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
            Encoding koi8u = Encoding.GetEncoding(21866);

            // Check errors
            string infile = "C:\\Users\\user\\source\\repos\\AVS\\LR7\\input.txt";
            var fi = new FileInfo(infile);
            if (!File.Exists(infile))
            {
                Console.WriteLine("Can't find input.txt");
                return;
            }
            else if (fi.Length == 0) 
            {
                Console.WriteLine("File input.txt is empty");
                return;
            }


            string[] asciistrs = File.ReadAllLines(infile, ascii);
            List<string> koi8ustrs_lst = new List<string>(); 

            foreach (string asciistr in asciistrs) {
                // Convert the string into a byte array.
                byte[] asciiBytes = ascii.GetBytes(asciistr);

                // Perform the conversion from one encoding to the other.
                byte[] koi8uBytes = Encoding.Convert(ascii, koi8u, asciiBytes);

                // Convert the new byte[] into a char[] and then into a string.
                char[] koi8uChars = new char[koi8u.GetCharCount(koi8uBytes, 0, koi8uBytes.Length)];
                koi8u.GetChars(koi8uBytes, 0, koi8uBytes.Length, koi8uChars, 0);
                koi8ustrs_lst.Add(new string(koi8uChars));

            }

            File.WriteAllLines("C:\\Users\\user\\source\\repos\\AVS\\LR7\\output.txt", koi8ustrs_lst.ToArray(), koi8u);
        }
    }
}