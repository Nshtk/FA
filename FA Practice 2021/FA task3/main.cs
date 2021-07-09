using System;
using System.Collections.Generic;

namespace Practice_Cs3
{
    class Cache<T>
    {
        public int max_size;
        public TimeSpan lifetime;
        Dictionary<string, Tuple<T, DateTime>> memory;
        public Cache(int size, TimeSpan time)
        {
            max_size = size;
            lifetime = time;
            memory = new Dictionary<string, Tuple<T, DateTime>>(this.max_size);
        }

        private void TryDelete()
        {
            foreach (var key in memory.Keys)
                if (lifetime < (DateTime.Now - memory[key].Item2))
                    memory.Remove(key);
        }
        public T Get(string key)
        {
            TryDelete();

            if (!memory.ContainsKey(key))
                throw new KeyNotFoundException();
            return memory[key].Item1;
        }
        public bool Save(string key, T data)
        {
            TryDelete();

            if (memory.ContainsKey(key))
                throw new ArgumentException($"Key {key} is already used.");

            if (memory.Count < max_size)
                memory.Add(key, new Tuple<T, DateTime>(data, DateTime.Now));
            else
            {
                TimeSpan time_remaining = lifetime;
                TimeSpan tmp_time;
                string min = null;

                foreach (var k in memory.Keys)
                {
                    tmp_time = DateTime.Now - memory[k].Item2;
                    if (tmp_time < time_remaining)
                    {
                        time_remaining = tmp_time;
                        min = k;
                    }
                }

                if (min == null)
                {
                    Console.WriteLine("Cache was just loaded, failed to write data.");
                    return false;
                }
                memory.Remove(min);
                memory.Add(key, new Tuple<T, DateTime>(data, DateTime.Now));
            }
            return true;
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Cache<int> cache = new Cache<int>(3, TimeSpan.FromSeconds(15));

            cache.Save("entry_one", 1);
            cache.Save("entry_two", 2);
            cache.Save("entry_three", 3);

            Console.WriteLine("Data: {0}", cache.Get("entry_two"));
        }
    }
}
