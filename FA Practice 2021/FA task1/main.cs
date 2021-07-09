using System;
using System.Collections.Generic;
using System.Linq;

namespace Practice_Cs1
{
    public static class MyExtension
    {
        public static void CheckRepeatebles(this IEnumerable<string> str_array, in IEqualityComparer<string> comperor)
        {
            IEnumerable<string> unique_str_array = str_array.Distinct(comperor);
            if (unique_str_array== str_array)                                               // Через Count сделать не получилось
                throw new ArgumentException();
        }

        public static IEnumerable<IEnumerable<string>> GetCombinationsList(this IEnumerable<string> str_array, int k, in IEqualityComparer<string> comperor)
        {
            CheckRepeatebles(str_array, in comperor);
            List<List<string>> objects = new List<List<string>>();

            foreach (var obj in GetCombinations(str_array.ToList(), k))
                objects.Add(obj.ToList());
            return objects;
        }
        private static IEnumerable<IEnumerable<string>> GetCombinations(List<string> str_array, int k)
        {
            if (k <= 0)
                yield return new List<string>();
            else
            {
                foreach (var str in str_array)
                {
                    foreach (var c in GetCombinations(str_array, k - 1))
                    {
                        List<string> das = new List<string>();
                        das.Add(str);
                        das.AddRange(c);
                        yield return das;
                    }
                }
            }     
        }
        public static IEnumerable<IEnumerable<string>> GetSubsetsList(this IEnumerable<string> str_array, in IEqualityComparer<string> comperor)
        {
            CheckRepeatebles(str_array, in comperor);
            List<IEnumerable<string>> subsets = new List<IEnumerable<string>>();
            List<string> t = str_array.ToList();

            for (int i = 0; i < t.Count * t.Count; i++)
            {
                List<string> combination = new List<string>(t.Count);

                for (int j = 0; j < t.Count; j++)
                    if ((i & (1 << (t.Count - j - 1))) != 0)
                        combination.Add(t[j]);
                subsets.Add(new List<string>(combination));
            }

            return subsets;
        }
        public static IEnumerable<IEnumerable<string>> GetPermutationsList(this IEnumerable<string> str_array, in IEqualityComparer<string> comperor)
        {
            CheckRepeatebles(str_array, in comperor);
            List<IEnumerable<string>> permutations = new List<IEnumerable<string>>();
            int count_minus_one = str_array.ToList().Count - 1;

            GetPermutations(str_array.ToList(), ref permutations, 0, count_minus_one);

            return permutations;
        }
        private static void GetPermutations(List<string> str_array, ref List<IEnumerable<string>> permutations, int k, int n)
        {
            if (k == n)
            {
                List<string> tmp = new List<string>();

                foreach (var str in str_array)
                    tmp.Add(str);
                permutations.Add(tmp);
            }
            else
            {
                for (int i = k; i <= n; i++)
                {
                    Swap(ref str_array, k, i);
                    GetPermutations(str_array, ref permutations, k + 1, n);
                    Swap(ref str_array, k, i);
                }
            }
        }
        private static void Swap(ref List<string> str_array, int a, int b)
        {
            if (str_array[a] == str_array[b])
                return;
            string temp = str_array[a];
            str_array[a] = str_array[b];
            str_array[b] = temp;
        }
    }
    class Program
    {
        public class MyEquality : IEqualityComparer<string>
        {
            public bool Equals(string a, string b)
            {
                if (a == null || b == null)
                    return false;
                return (a == b);
            }
            public int GetHashCode(string str)
            {
                return str.GetHashCode();
            }
        }
        public static void Print(IEnumerable<string> obj)
        {
            foreach (var str in obj)
                Console.Write(str + ' ');
            Console.WriteLine();
        }
        public static void PrintMyCollectionList(in IEnumerable<IEnumerable<string>> arr, string text)
        {
            Console.WriteLine(text);
            foreach (var obj in arr)
                Print(obj);
            Console.WriteLine();
        }

        static void Main(string[] args)
        {
            IEqualityComparer<string> comp = new MyEquality();
            IEnumerable<IEnumerable<string>> objects_first = new List<List<string>>();
            IEnumerable<IEnumerable<string>> objects_second = new List<List<string>>();
            IEnumerable<IEnumerable<string>> objects_third = new List<List<string>>();

            List<string> data = new List<string>(new string[] { "1", "2", "3"});

            try
            {
                objects_first = data.GetCombinationsList(3, in comp);
                PrintMyCollectionList(in objects_first, "Combinations:");

                objects_second = data.GetSubsetsList(in comp);
                PrintMyCollectionList(in objects_second, "Subsets:");

                objects_third = data.GetPermutationsList(in comp);
                PrintMyCollectionList(in objects_third, "Permutations:");
            }
            catch (ArgumentException e)
            {
                Console.WriteLine("Repeatables were found");
            }
            
        }
    }
}
