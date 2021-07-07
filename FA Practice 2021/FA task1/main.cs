using System;
using System.Collections;
using System.Collections.Generic;

namespace Practice_Cs1
{
    class Program
    {
        public class MyCollectionEquality : IEqualityComparer<string>, IEqualityComparer<MyCollection>
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
            public bool Equals(MyCollection a, MyCollection b)
            {
                if (a is null || b is null)
                    return false;
               
                return (a.StrArray == b.StrArray);
            }
            public int GetHashCode(MyCollection obj)
            {
                return obj.GetHashCode();
            }
        }
        public class MyCollection : IEnumerable<string>
        {
            private List<string> str_array = new List<string>();

            public MyCollection()
            {
                str_array = new List<string>();
            }
            public MyCollection(List<string> arr)
            {
                str_array = arr;
            }
            public MyCollection(string str, MyCollection c)
            {
                str_array.Add(str);
                List<string> tmp=c.StrArray;
                foreach (var s in tmp)
                    str_array.Add(s);
            }

            public List<string> StrArray
            {
                get => str_array;
            }

            public void CheckRepeatebles(in MyCollectionEquality comperor)
            {
                int t;

                foreach (var a in str_array)
                {
                    t = 0;
                    foreach (var b in str_array)
                        if (comperor.Equals(a, b))
                            t++;
                    if (t > 1)
                        throw new ArgumentException(String.Format("{0} is repeatable element", a));
                }
            }
            public void Print()
            {
                foreach (var str in this)
                    Console.Write(str + ' ');
                Console.WriteLine();
            }

            public List<MyCollection> GetCombinationsList(int k, in MyCollectionEquality comperor)
            {
                CheckRepeatebles(in comperor);
                List<MyCollection> objects = new List<MyCollection>();

                foreach (var obj in GetCombinations(k))
                    objects.Add(obj);
                return objects;
            }
            private IEnumerable<MyCollection> GetCombinations(int k)
            {
                if (k <= 0)
                    yield return new MyCollection();
                else
                    foreach (var str in str_array)
                        foreach (var c in GetCombinations(k - 1))
                            yield return new MyCollection(str, c);
            }
            public List<MyCollection> GetSubsetsList(in MyCollectionEquality comperor)
            {
                CheckRepeatebles(in comperor);
                List<MyCollection> subsets = new List<MyCollection>();

                for (int i = 0; i < str_array.Count*str_array.Count; i++)
                {
                    List<string> combination = new List<string>(str_array.Count);

                    for (int j = 0; j < str_array.Count; j++)
                        if ((i & (1 << (str_array.Count - j - 1))) != 0)
                            combination.Add(str_array[j]);
                    subsets.Add(new MyCollection(combination));
                }

                return subsets;
            }
            public List<MyCollection> GetPermutationsList(in MyCollectionEquality comperor)
            {
                CheckRepeatebles(in comperor);
                List<MyCollection> permutations = new List<MyCollection>();
                int count_minus_one = str_array.Count - 1;

                GetPermutations(ref permutations, 0, count_minus_one);

                return permutations;
            }
            private void GetPermutations(ref List<MyCollection> permutations, int k, int n)
            {
                if (k == n)
                {
                    MyCollection tmp = new MyCollection();

                    foreach (var str in str_array)
                        tmp.StrArray.Add(str);
                    permutations.Add(tmp);
                }
                else
                {
                    for (int i = k; i <= n; i++)
                    {
                        Swap(k, i);
                        GetPermutations(ref permutations, k + 1, n);
                        Swap(k, i);
                    }
                }    
            }
            private void Swap(int a, int b)
            {
                if(str_array[a] == str_array[b]) 
                    return;
                string temp = str_array[a];
                str_array[a] = str_array[b];
                str_array[b] = temp;
            }
            
            public IEnumerator<string> GetEnumerator()
            {
                return new MyEnumerator(str_array);
            }
            IEnumerator IEnumerable.GetEnumerator()
            {
                return GetEnumerator();
            }
        }
        public class MyEnumerator : IEnumerator<string>
        {
            private List<string> str_array;
            int pos=-1;
            public MyEnumerator(List<string> arr)
            {
                str_array=arr;
            }

            public string Current
            {
                get
                {
                    if (pos == -1 || pos >= str_array.Count)
                        throw new InvalidOperationException();
                    return str_array[pos];
                }
            }
            public bool MoveNext()
            {
                if (pos < str_array.Count - 1)
                {
                    pos++;
                    return true;
                }
                else
                    return false;
            }
            public void Reset()
            {
                pos = -1;
            }

            object IEnumerator.Current => throw new NotImplementedException();
            public void Dispose() {}
        }

        public static void PrintMyCollectionList(in List<MyCollection> arr, string text)
        {
            Console.WriteLine(text);
            foreach (var obj in arr)
                obj.Print();
            Console.WriteLine();
        }

        static void Main(string[] args)
        {
            MyCollectionEquality comp= new MyCollectionEquality();
            List<MyCollection> objects_first = new List<MyCollection>();
            List<MyCollection> objects_second = new List<MyCollection>();
            List<MyCollection> objects_third = new List<MyCollection>();

            List<string> data = new List<string>(new string[] { "1", "2", "3"});
            MyCollection obj = new MyCollection(data);

            objects_first = obj.GetCombinationsList(3, in comp);
            PrintMyCollectionList(in objects_first, "Combinations:");

            objects_second = obj.GetSubsetsList(in comp);
            PrintMyCollectionList(in objects_second, "Subsets:");

            objects_third = obj.GetPermutationsList(in comp);
            PrintMyCollectionList(in objects_third, "Permutations:");
        }
    }
}
