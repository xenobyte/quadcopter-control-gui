# quadcopter-control-gui
A software prototype for a graphical user interface to control engines of a quadcopter. This project is splitted into two subprojects

* QuadcopterGui: WPF-Application written in C# to define values like angular velocity. 
* QuadServer with is a tcp server running on the quadcopter. It's written in C++ and parses the messages from the gui and controls the engines.

These components communicate over tcp/ip using json messages.


-----------------     json        --------------
| QuadcopterGui |   -------->     | QuadServer |
-----------------     tcp         --------------
