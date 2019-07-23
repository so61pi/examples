.. contents:: Table of Contents

Creational Patterns
===================

    Creational design patterns are design patterns that deal with **object creation mechanisms**, trying to create objects in a manner suitable to the situation.

    --- https://en.wikipedia.org/wiki/Creational_pattern

Abstract Factory
----------------

An abstract factory that creates abstract products.

    The Abstract Factory design pattern solves problems like:

    - How can an application be independent of how its objects are created?
    - How can a class be independent of how the objects it requires are created?
    - How can families of related or dependent objects be created?

    --- https://en.wikipedia.org/wiki/Abstract_factory_pattern

Builder
-------

Put materials in one by one, then build.

.. code-block:: text

    product1 = builder.setA('A').build()
    product2 = builder.setA('A').setB('B').build()

``builder``, ``product1`` and ``product2`` can be concrete or abstract classes.

    The Builder design pattern solves problems like:

    - How can a class (the same construction process) create different representations of a complex object?
    - How can a class that includes creating a complex object be simplified?

    --- https://en.wikipedia.org/wiki/Builder_pattern

Factory Method
--------------

.. code-block:: text

    product = factoryMethod(enum productType)

``product`` is an abstract class.

    The Factory Method design pattern solves problems like:

    - How can an object be created so that subclasses can redefine which class to instantiate?
    - How can a class defer instantiation to subclasses?

    --- https://en.wikipedia.org/wiki/Factory_method_pattern

Prototype
---------

Create a prototype object, create other objects by cloning it.

    The Prototype design pattern solves problems like:

    - How can objects be created so that which objects to create can be specified at run-time?
    - How can dynamically loaded classes be instantiated?

    --- https://en.wikipedia.org/wiki/Prototype_pattern

Lazy Initialization
-------------------

Create objects only when they are first needed, not when the ingredients are available.

Object Pool
-----------

Building and destroying a toilet on demand is time consuming, better to have several of them ready in a restroom.

Structural Patterns
===================

    Structural design patterns are design patterns that ease the design by identifying a simple way to realize **relationships among entities**.

    --- https://en.wikipedia.org/wiki/Structural_pattern

Adapter
-------

When you try to fit an existing class into an existing interface.

    The adapter design pattern solves problems like:

    - How can a class be reused that does not have an interface that a client requires?
    - How can classes that have incompatible interfaces work together?
    - How can an alternative interface be provided for a class?
    
    --- https://en.wikipedia.org/wiki/Adapter_pattern

Bridge
------

On class for Linux, one class for Windows, and a class to hide them so you don't have to care about the horror.

    What problems can the Bridge design pattern solve?

    - An abstraction and its implementation should be defined and extended independently from each other.
    - A compile-time binding between an abstraction and its implementation should be avoided so that an implementation can be selected at run-time.

    --- https://en.wikipedia.org/wiki/Bridge_pattern

Composite
---------

To build a tree, everything is a node and has the same interface.

    What problems can the Composite design pattern solve?

    - A part-whole hierarchy should be represented so that clients can treat part and whole objects uniformly.
    - A part-whole hierarchy should be represented as tree structure.

    --- https://en.wikipedia.org/wiki/Composite_pattern

Decorator
---------

Decorate an object with new functionalities while preserving the old ones.

    What problems can it solve?

    - Responsibilities should be added to (and removed from) an object dynamically at run-time.
    - A flexible alternative to subclassing for extending functionality should be provided.

    --- https://en.wikipedia.org/wiki/Decorator_pattern

Facade
------

Group complex operations into one function/class for ease of use.

    What problems can the Facade design pattern solve?

    - To make a complex subsystem easier to use, a simple interface should be provided for a set of interfaces in the subsystem.
    - The dependencies on a subsystem should be minimized.

    -- https://en.wikipedia.org/wiki/Facade_pattern

Flyweight
---------

Multiple objects share a portion of memory to save memory, like copy-on-write strings.

    What problems can the Flyweight design pattern solve?

    - Large numbers of objects should be supported efficiently.
    - Creating large numbers of objects should be avoided.

    --- https://en.wikipedia.org/wiki/Flyweight_pattern

Proxy
-----

Just a wrapper to an existing object with additional checkings/functionalities.

    What problems can the Proxy design pattern solve?

    - The access to an object should be controlled.
    - Additional functionality should be provided when accessing an object.

    --- https://en.wikipedia.org/wiki/Proxy_pattern

Bahavioral Patterns
===================

    Behavioral design patterns are design patterns that identify common **communication patterns among objects** and realize these patterns.

    --- https://en.wikipedia.org/wiki/Behavioral_pattern

Chain Of Responsibility
-----------------------

A chain of handlers (``handler0 -> handler1 -> ... -> handlerN``), if ``handlerI`` can process a request, then handle it and stop, else pass it on.

    What problems can the Chain of Responsibility design pattern solve?

    - Coupling the sender of a request to its receiver should be avoided.
    - It should be possible that more than one receiver can handle a request.

    --- https://en.wikipedia.org/wiki/Chain-of-responsibility_pattern

Command
-------

An object that holds enough information (inputs + action receiver) to carry out an action.

    What problems can the Command design pattern solve?

    - Coupling the invoker of a request to a particular request should be avoided. That is, hard-wired requests should be avoided.
    - It should be possible to configure an object (that invokes a request) with a request.

    --- https://en.wikipedia.org/wiki/Command_pattern

Interpreter
-----------

Basically, an interpreter.

    What problems can the Interpreter design pattern solve?

    - A grammar for a simple language should be defined
    - so that sentences in the language can be interpreted.

    --- https://en.wikipedia.org/wiki/Interpreter_pattern

Iterator
--------

Basically, an iterator.

    What problems can the Iterator design pattern solve?

    - The elements of an aggregate object should be accessed and traversed without exposing its representation (data structures).
    - New traversal operations should be defined for an aggregate object without changing its interface.

    --- https://en.wikipedia.org/wiki/Iterator_pattern

Mediator
--------

An object that encapsulates the interaction logic between other objects.

    What problems can the Mediator design pattern solve?

    - Tight coupling between a set of interacting objects should be avoided.
    - It should be possible to change the interaction between a set of objects independently.

    --- https://en.wikipedia.org/wiki/Mediator_pattern

Memento
-------

Snapshot and restore.

    What problems can the Memento design pattern solve?

    - The internal state of an object should be saved externally so that the object can be restored to this state later.
    - The object's encapsulation must not be violated.

    --- https://en.wikipedia.org/wiki/Memento_pattern

Observer/Pub-Sub
----------------

Pub-Sub with or without topic.

    What problems can the Observer design pattern solve?

    - A one-to-many dependency between objects should be defined without making the objects tightly coupled.
    - It should be ensured that when one object changes state an open-ended number of dependent objects are updated automatically.
    - It should be possible that one object can notify an open-ended number of other objects.

    --- https://en.wikipedia.org/wiki/Observer_pattern

State
-----

Question: Is this pattern not so useful?

    The state pattern is set to solve two main problems:

    - An object should change its behavior when its internal state changes.
    - State-specific behavior should be defined independently. That is, adding new states should not affect the behavior of existing states.

    --- https://en.wikipedia.org/wiki/State_pattern

Strategy
--------

Let users of a class provide different algorithms for some internal parts of said class. An example is `Policy-based design in C++ <https://en.wikipedia.org/wiki/Modern_C++_Design#Policy-based_design>`__.

*Note:* This pattern looks really similar to dependency injection, but there are some different points:

- Usually, there are more than one strategy.
- During object's lifetime, one strategy can be replaced with another.
- Dependency injection can be done for testing purpose only.

Dependency Injection
--------------------

Class receives its dependencies from its constructor instead of creating them itself.

    The Dependency Injection design pattern solves problems like:

    - How can an application or class be independent of how its objects are created?
    - How can the way objects are created be specified in separate configuration files?
    - How can an application support different configurations?

    --- https://en.wikipedia.org/wiki/Dependency_injection

Template Method
---------------

Having a skeleton of an algorithm, but let subclasses decide some points. Like `private virtual function in C++ <http://www.gotw.ca/publications/mill18.htm>`__.

Visitor
-------

When you have an operation that needs to be performed on multiple (but similar) objects through their public methods, you are a visitor, 

    What problems can the Visitor design pattern solve?

    - It should be possible to define a new operation for (some) classes of an object structure without changing the classes.

    --- https://en.wikipedia.org/wiki/Visitor_pattern

.. code-block:: text

    - EA EB EC: Object types that need to be drawn.
    - VX VY VZ: The ways to draw objects.

    - E.accept(V): Draw object E using way V.
    - V.visitA(A): Use public methods of A to draw it using way V.

         +-- EA
    E <--+-- EB
         +-- EC

         +-- VX
    V <--+-- VY
         +-- VZ

    E.accept(V)
             V.visit

       |---> X.visitA --+
    A -|---> Y.visitA   |
       |---> Z.visitA   |
                        |
       |---> X.visitB --+
    B -|---> Y.visitB   |
       |---> Z.visitB   |
                        |
       |---> X.visitC --+
    C -|---> Y.visitC
       |---> Z.visitC


    struct EA {
        void accept(V) {
            V.visitA(this);
        }
    }

    struct EB {
        void accept(V) {
            V.visitB(this);
        }
    }


    struct VX {
        void visitA() { ... }
        void visitB() { ... }
        void visitC() { ... }
    }

    struct VY {
        void visitA() { ... }
        void visitB() { ... }
        void visitC() { ... }
    }

Concurrency Patterns
====================

Double-checked Locking
----------------------

Save you a little time for common case.

.. code-block:: c

    void func() {
        // p is non-null most of the time.
        if (!p) {
            lock(mutex);
            if (!p) {
                initialize(p);
            }
            unlock(mutex);
        }
    }

Reactor
-------

https://en.wikipedia.org/wiki/Reactor_pattern

Multiple-readers/Single-writer Lock
-----------------------------------

https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock

Architectural Patterns
======================

Remote Procedure Call
---------------------

Model-View-Controller
---------------------

https://stlab.cc/tips/about-mvc.html

Pipes & Filters
---------------

Input of one stage is the output of previous stage, like ``Text -> Lexer -> Parser -> Intermediate Code Generator -> ...``.

https://en.wikipedia.org/wiki/Pipeline_(software)

Protocol Stack/Layers
---------------------

Separate your application into layers so that:

- A layer only depends on layers right beneath it.
- A layer has no knowledge about any upper layers.

Front Controller
----------------

https://en.wikipedia.org/wiki/Front_controller

References
==========

- Design Patterns: Elements of Reusable Object-Oriented Software
- Code Complete: A Practical Handbook of Software Construction, Second Edition
- Patterns of Enterprise Application Architecture
- Pattern-Oriented Software Architecture
- Enterprise Integration Patterns - Designing, Building And Deploying Messaging Solutions
- https://en.wikipedia.org/wiki/Software_design_pattern
