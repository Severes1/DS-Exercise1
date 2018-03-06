#define CHECK(x, y) do { \
      int retval = (x); \
      if (retval < 0) { \
              fprintf(stderr, "Runtime error: %s returned %d at %s:%d", #x, retval, __FILE__, __LINE__); \
              y; \
            } \
} while (0)

// We're returning a different error code
// CHECK(someFunction1(foo), return someErrorCode);
// // We're actually calling it from C++ and can throw an exception
// CHECK(someFunction2(foo), throw SomeException("someFunction2 failed")):
