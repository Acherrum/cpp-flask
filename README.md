# cpp-flask
A C++ variant of Python Flask, including a HtmlBuilder, JSON library and Webserver, for fast and flexible webapps with a C++ backend.

## TL;DR
This project can be build and installed to provide an easy to use webserver.
It supports HTML template building and programming (similar to jinja2).
See the examples, for a Hello World implementation to get started quickly.
More examples will follow as the project matures (voicing intend, not a promise).

## Project structure
The project contains 3 internal libraries and exports 1 bundle upon installation.

The internal libraries are
1. HttpServer 
   - Simple entry point to set up and handle the API.
   - Currently handles POST and GET routes.
2. HtmlBuilder
   - Conditionals (if, else if, else)
   - Loops (classic for-loop and for .. in .. )
   - Variables (fetched from JsonObject)
   - Setting custom variables (when you want to calculate something in the frontend)
   - Including other files (keep things DRY)
3. JsonObject
   - A user-friendly wrapper-object around RapidJSON

Because this structure is mostly for internal architecture (keeping things organized),
a single target will be available for end-users upon installation: CppFlask.

## Installation

Simply running CMake build steps, should install the project and make it available for 
usage in your CMake project.

```aiignore
mkdir -p build && cd build
cmake ..
cmake --build . --target install
```

## Examples

In the examples directory, a simple Hello World project is set up.
The way it works:

- An executable is created, which runs the server.
- The main method creates the HttpServer backend and injects a custom Router.
- The Router describes the different API routes that are possible.
  - At the time of writing, only get/post actions are implemented.
- Information from forms is automatically passed into a JsonObject, for ease of use.

## Credits

This project is build on yhirose's cpp-httplib: https://github.com/yhirose/cpp-httplib

The idea to build this came from a C++ beginners class I taught in the summer of 2026, where a group of my students
decided they wanted to build a webapp using httplib. This lead me to try out httplib for myself
and while it worked, I found the syntax required a lot of boilerplate code and forms access
felt unhandy.
Furthermore, some students in this group had never typed any HTML code before.
This is where the idea of the HtmlBuilder library came from.
I already had experience with Python Flask and with these ideas combined, I set out to build a
C++ version of it. The end goal: refactor some of my local webapps into this C++ version.

Furthermore, I have included a custom JsonObject library, which is a slim wrapper around Tencent's RapidJSON: https://github.com/Tencent/rapidjson
I find RapidJSON very user unfriendly, so this object makes it easier to work with.

Finally, I've written some tests using GoogleTest framework: https://github.com/google/googletest
Although I do advocate TDD, I am still an engineer who likes to build as he goes and tests mainly for my own sanity.

## Work in progress

This is a hobby project.
For now, I will not accept any contributions.
Feel free to fork, but this is, for the time being, something to keep myself busy.
Thank you for understanding. 

## Version history

- **0.2.0** TBD
  - Finished nested routes.
  - Added nested route with JSON response to example.
  - Add `isEmpty()` method to JsonObject.
  - Add method to add objects to JsonObject.
  - Fix parse error when END_IF is missing.
- **0.1.0** 2026-09-11
  - Initial release on Github, with this README and a first example.