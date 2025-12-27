# Alpha's UI Pack

Provides a bunch of extra nodes and tools:

## AdvancedScrollLayer
  - Improved Scroll Layer compared to Cocos, Geode, and Geometry Dash's implementation.
  - Inertia scroll
  - Proper horizontal scroll
  - Keyboard scrolling
  - Top Left origin based
  - Scroll Units instead of manual content positioning
  - Easy to use
## AdvancedScrollBar
  - Scrolls the AdvancedScrollLayer
## RenderNode
  - Wraps a node and renders it to this as a sprite.
## TouchBlocker
  - Easily block touch based on another node's bounds.
## CursorManager 
  - Allows changing the mouse cursor easily
## TouchDispatcher 
  - Brings proper hover controls as well as Right, Middle, Button 3, and Button 4 click support.
  - Supports normal touch prio, staying consistent with Geometry Dash.

You can include everything with:
```cpp
#include <alphalaneous.alphas-ui-pack/include/API.hpp>
```
or add each header as needed. 

Similarly, you can bring every namespace to the scope with
```cpp
using namespace alpha::prelude;
```
or use each namespace as needed.

**View the headers for proper documentation.**

You can view the examples folder for some example code on how to use the provided classes.