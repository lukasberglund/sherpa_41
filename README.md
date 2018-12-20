A toy browser engine, built loosely from [Limpet's _Let's build a browser engine!_](https://limpet.net/mbrubeck/2014/08/08/toy-layout-engine-1.html)
with architectures introduced in [CS3251](https://vuse-cs3251.github.io).

This app makes use of modern `C++` and established design patterns to create
an engine that is fairly trivial to browse and extend. Note, however, that the
engine is ___not___ designed to be robust, or even useful.

### Notes

- JavaScript is turned off by default, and can't be turned on. You may say JavaScript is unsupported. I say you're right.

- Please don't try to break the application, because it will. Pretty easily. There is ~just enough~ error checking to recover trivial mistakes, but auto-closing tags and the like is not supported.