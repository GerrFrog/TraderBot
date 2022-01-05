# Asynchronous

Implementatino of parallel functions execution in C++

```c
 1 #include <future>
 2 #include <iostream>
 3 
 4 void called_from_async() {
 5   std::cout << "Async call" << std::endl;
 6 }
 7 
 8 int main() {
 9   //called_from_async launched in a separate thread if possible
10   std::future<void> result( std::async(called_from_async));
11 
12   std::cout << "Message from main." << std::endl;
13 
14   //ensure that called_from_async is launched synchronously
15   //if it wasn't already launched
16   result.get();
17 
18   return 0;
19 }
```

```c
 1 #include <future>
 2 #include <iostream>
 3 
 4 int main() {
 5   //called_from_async launched in a separate thread if possible
 6   std::future<int> result( std::async([](int m, int n) { return m + n } , 2, 4));
 7 
 8   std::cout << "Message from main." << std::endl;
 9 
10   //retrive and print the value stored in the future
11   std::cout << result.get() << std::endl;
12 
13   return 0;
14 }
```

```c
 1 #include <future>
 2 #include <iostream>
 3 #include <vector>
 4 
 5 int twice(int m) {
 6   return 2 * m;
 7 }
 8 
 9 int main() {
10   std::vector<std::future<int>> futures;
11 
12   for(int i = 0; i < 10; ++i) {
13     futures.push_back (std::async(twice, i));
14   }
15 
16   //retrive and print the value stored in the future
17   for(auto &e : futures) {
18     std::cout << e.get() << std::endl;
19   }
20 
21   return 0;
22 }
```

```c
 1 #include <future>
 2 #include <iostream>
 3 #include <vector>
 4 
 5 int main() {
 6   std::vector<std::future<int>> futures;
 7 
 8   for(int i = 0; i < 10; ++i) {
 9     futures.push_back (std::async([](int m) {return 2 * m;} , i));
10   }
11   ...
12 }
```

```c
 1 int main() {
 2   std::vector<std::future<void>> futures;
 3   int frames = 1800;
 4   int id_width = 4;
 5   double delta = 1.0 / (double) frames;
 6 
 7 
 8   auto start = std::chrono::steady_clock::now();
 9   for(int id = 0; id <= frames; ++id) {
10     double z = (double) id * delta;
11     futures.push_back(std::async(make_perlin_noise, id, id_width, z));
12   }
13 
14   for(auto &e : futures) {
15       e.get();
16   }
17   auto end = std::chrono::steady_clock::now();
18 
19   auto diff = end - start;
20   std::cout << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
21 
22   return 0;
23 }
```


