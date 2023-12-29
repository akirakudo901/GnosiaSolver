# **Gnosia Solver**

## **Purpose**

This project is my **first try** creating a simple functional program with ***C++***.

* Also my first encounter with **smart pointers**!

* It implements a **Constraint Satisfaction Problem Solver** using the **Arc Consistency #3 algorithm**.

    (Was taught "Arc consistency" which seems to match AC-3 descrisiption from Wikipedia[https://en.wikipedia.org/wiki/AC-3_algorithm]).

* This is applied to the visual novel RPG **"GNOSIA"** [https://www.gnosia-game.com/], a **werewolf-style social deduction game**, to obtain solutions.

## **Project Structure**
* Find an implementation of a **graph object** under **src/graphImplementation**.

* Find an implementation of the **CSP solver** using the graph object defined above under **src/cspSolver**.

* Find the **application of the CSP solver to GNOSIA** under **src/gnosiaSolver**.

## **How GNOSIA is solved**

### Constraint Satisfaction Problem (CSP)
The deduction component of GNOSIA is a case of a **Constraint Satisfaction Problem (CSP)**, a problem that can be specified in terms of:

* A **set of variables** taking different values within their domains.
* A **set of constraints** applying to possible values variables can take.

A **"solution"** to such a CSP problem is a **successful assignment** of **domain values** to **every variable**, such that **no constraint is violated**.

### GNOSIA as CSP
If you know the game, GNOSIA specifically can be modeled as follows:
* Each of the **crew member** is a **variable**, sharing the domain of all 8 possible roles:
    
    AC Follower | Bug | Crew Member | Doctor | Engineer | Gnosia | Guardian Angel | Guard Duty

* Many complex relationships that hold in the game, such as the **number of possible true Doctors**, are modeled as **constraints** between domain values of variables.

* Information revealed as the game progresses will add further constraints of their possible domains.

    e.g. A crew member disappearing automatically rules out the possibility they are a Gnosia.

One can solve a CSP problem defined as the above to get a certain grasp of the current situation.
