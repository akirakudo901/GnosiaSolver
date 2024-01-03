// Author: Akira Kudo

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

#include "src/cspSolver/CSPGraphCreator.h"
#include "src/cspSolver/CSPGraph.h"

using namespace CSPSolverImplementation;
using std::cout, std::cin, std::endl;

// calling this method initiates graph creation in the CLI
CSPGraph CSPGraphCreator::StartCSPGraphCreator() 
{
    keepGoing = true;
    returnedGraph = CSPGraph();

    cout << "Initiating graph creation.\n";

    auto print_choice_menu = [] () {
        cout << "Select from the following: " << endl;
        cout << " 1) Add a constraint: AC" << endl;
        cout << " 2) Add a variable  : AV" << endl;
        cout << " 3) Add an edge     : AE" << endl;
        cout << " 4) Remove a vertex : RV" << endl;
        cout << " 5) Remove an edge  : RE\n" << endl;
    };

    print_choice_menu();

    std::string line;
    do {
        cout << "GraphCreation > ";
        std::getline(cin, line);
        // process the obtained line
        if (!line.empty()) { 
            process(line);

            cout << "\nCSP Graph so far:\n";
            cout << returnedGraph;

            if (!keepGoing) break;

            print_choice_menu();
        }
    }
    while (keepGoing);

    cout << "The final CSP Graph was:\n";
    cout << returnedGraph;
    cout << "Ending graph creation!\n";
    
    return returnedGraph;
};

// specifies how CLI inputs should be handled
void CSPGraphCreator::process(std::string const & line)
{
    // keep asking for user input until getting an integer input between min and max inclusive
    auto get_valid_int_input = [] (std::string message, int min=0, int max=100) {
        int returned;

        // loop to check if int between min and max inclusive is passed
        do {
            // print first message and get input
            cout << message && cin >> returned;
            // check if it is a valid integer
            if (!cin.good()) {
                cout << "Input wasn't an int; try again!\n";
                // clear invalid input flag in cin
                cin.clear();
            } else if (min > returned || returned > max) {
                cout << "Given integer wasn't within the range [" << min << "-" << max << "]."\
                     << "Enter a value in the valid range!" << endl;
            } else { //otherwise, if we reach this point, we have a valid input
                break;
            }
            // if we are scanning again, skip the current input to cin
            cin.ignore(10000,'\n');
        }
        while (true);

        // clearing cin just in case
        cin.ignore(10000,'\n');
        return returned;
    };

    auto get_valid_yes_or_no_input = [] (std::string message) {
        bool returned;
        std::string user_input;

        // loop to check if input matches any of the commands
        do {
            // print first message and get input
            cout << message && cin >> user_input;

            // check if it is a valid string
            if (!cin.good()) {
                cout << "Input wasn't a recognized string; try again!\n";
                // clear invalid input flag in cin
                cin.clear();
                // skip the current input to cin
                cin.ignore(10000,'\n');
                continue;
            }

            // otherwise, input is a string, so scan for command
            // remove all white spaces
            user_input.erase (std::remove (user_input.begin(), user_input.end(), ' '), user_input.end());
            // change returned to all uppercase
            std::transform(user_input.begin(), user_input.end(), user_input.begin(), ::toupper);
    
            // check if it matches the "yes" commands
            if (user_input == "Y" || user_input == "YES") {
                returned = true;
                break;
            // check if it matches the "no" commands
            } else if (user_input == "N" || user_input == "NO") {
                returned = false;
                break;
            } else { //otherwise, if we reach this point, we have a valid input
                cout << "Input wasn't any of Y/YES/N/NO (case insensitive). Try again!" << endl;
            }

            // if we are scanning again, skip the current input to cin
            cin.ignore(10000,'\n');
        }
        while (true);

        // clearing cin just in case
        cin.ignore(10000,'\n');
        return returned;
    };
    /*
    What do you want to do?

    - add a new vertex: name, domain
    - add a new edge between two vertices (by name)
    - remove a vertex by name
    - remove an edge between two vertices
    */
    std::string lineCopy = line;
    std::transform(lineCopy.begin(), lineCopy.end(), lineCopy.begin(), ::toupper);
    // if scanned line isn't part of the commands, indicate that and return
    if (stringToEnum.find(lineCopy) == stringToEnum.end()) {
        cout << "Command: " << lineCopy << " not recognized, try again!\n";
        return;
    }
    // otherwise, parse the command
    StringValue correspondingEnum = stringToEnum[lineCopy];

    switch (correspondingEnum) {
        int choice;

        case evAddConstraint:
        {
            std::string cv_name;
            std::function<bool(int, std::initializer_list<GraphImplementation::VariableVertex>)> pred;
            std::string description;

            int domain_val; int n;
            // creating constraint
            cout << "Creating a constraint." << endl;
            // get the name
            cout << "Enter constraint name: " && cin >> cv_name;
            cout << "Constraint name is now: " << cv_name << "." << endl;
            // then the type of constraint
            choice = get_valid_int_input("Enter constraint type |1) >= |2) <= |3) = : ", 1, 3);
            cout << "Choice of constraint type was: " 
                 << ((choice==1) ? ">=" : ((choice==2) ? "<=" : "=")) << "!" << endl;
            // the domain value the constraint checks
            domain_val = get_valid_int_input("Enter checked domain value: ");
            cout << "Choice of domain value was: " << domain_val << "!" << endl;
            // n associated with this constraint
            n = get_valid_int_input("Enter n: ");
            cout <<"Choice of n was: " << n << "!" << endl;
            // finally the description
            // n associated with this constraint
            cout << "Enter description: " && std::getline(cin, description);
            cout << "Given description was: " << description << "." << endl;

            switch (choice) {
                case 1:
                    pred = GraphImplementation::ConstraintVertex::greaterOrEqualToN(domain_val, n);
                    break;
                case 2:
                    pred = GraphImplementation::ConstraintVertex::lesserOrEqualToN(domain_val, n);
                    break;
                case 3:
                    pred = GraphImplementation::ConstraintVertex::exactlyN(domain_val, n);
                    break;
                default:
                    cout << "The choice was invalid; defaulting to '='." << endl;
                    pred = GraphImplementation::ConstraintVertex::exactlyN(domain_val, n);
                    break;
            }
            returnedGraph.add_constraint(cv_name, pred, description);
            cout << "Successfully added: constraint " << cv_name << " with type "\
                 << ((n==1) ? ">=" : ((n==2) ? "<=" : "=")) << ", checking domain "
                 << domain_val << " for n : " << n  <<"!" << endl;
            break;
        }
        case evAddVariable:
        {
            std::string vv_name;
            
            std::set<int> domain; int domain_val;
            // creating variable
            cout << "Creating a variable." << endl;
            // get the name
            cout << "Enter variable name: " && cin >> vv_name;
            cout << "Variable name is now: " << vv_name << "." << endl;

            // then what to insert in the domain
            bool add_more_domain_value = true;
            
            // first ask if we want to add more domain values
            add_more_domain_value = get_valid_yes_or_no_input("Would you like to add values to the domain?");
            
            // if yes, add them in a loop 
            while (add_more_domain_value) {
                domain_val = get_valid_int_input("Enter a new domain value to insert: ");
                cout << "Choice of domain value was: " << domain_val << "!" << endl;
                domain.insert(domain_val);
                // then ask again if we want more values
                add_more_domain_value = get_valid_yes_or_no_input("Would you like to add values to the domain?");
            }
            cout << "Final domain content was: { ";
            for (const int& d_val : domain) cout << d_val << ' '; 
            cout << "}!" << endl;
            // finally add the variable just specified
            returnedGraph.add_variable(vv_name, domain);
            break;
        }
        case evAddEdge:
        {
            std::string name1; std::string name2;
            
            // obtaining the two names
            cout << "Creating an edge." << endl;
            // get name1
            cout << "Enter name for variable vertex you want to link: " && cin >> name1;
            cout << "Vertex name is now: " << name1 << "." << endl;
            // get name2
            cout << "Enter name for constraint vertex you want to link: " && cin >> name2;
            cout << "Constraint name is now: " << name2 << "." << endl;
            // add edge
            returnedGraph.add_edge(name1, name2);
            cout << "Attempted addition of edge from " << name1 << " to " << name2 << ".";
            break;
        }
        case evRemoveVertex:
        {
            std::string name;
            // obtaining name of removed vertex
            cout << "Removing a vertex." << endl;
            // get name
            cout << "Enter name for vertex to remove: " && cin >> name;
            cout << "Vertex name is now: " << name << "." << endl;
            // attempt removal
            returnedGraph.remove_vertex(name);
            cout << "Attempted removal of vertex " << name << ".";
            break;
        }
        case evRemoveEdge:
        {
            std::string name1; std::string name2;

            // obtaining the two names
            cout << "Removing an edge." << endl;
            // get name1
            cout << "Enter name for variable vertex you want to unlink: " && cin >> name1;
            cout << "Vertex name is now: " << name1 << "." << endl;
            // get name2
            cout << "Enter name for constraint vertex you want to unlink: " && cin >> name2;
            cout << "Constraint name is now: " << name2 << "." << endl;
            // add edge
            returnedGraph.remove_edge(name1, name2);
            cout << "Attempted removal of edge from " << name1 << " to " << name2 << ".";   
            break;
        }
        case evExit:
            keepGoing = false;
            break;
    }
};