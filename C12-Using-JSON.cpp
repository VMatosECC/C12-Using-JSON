#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

void experiment01();
void experiment02();


class Person {
public:
    int    id;
    string name;
    int    age;

    //Lazy constructor
    Person(int id=0, const string& name="n.a.", int age=0) : id(id), name(name), age(age) {}

    // Serialize Person object to JSON
    json to_json() const {
        json j = json{ {"id", id}, {"name", name}, {"age", age} };
        return j;
    }

    // Deserialize JSON to Person object
    static Person from_json(const json& j) {
        return Person(j["id"].get<int>(), j["name"].get<string>(), j["age"].get<int>());
    }

    //Stringify the Person object
    string toString() const {
        stringstream sout;
        sout << "Person[Id: " << id << ", Name: " << name << ", Age: " << age << "]";
        return sout.str();
    }
};

//----------------------------------------------------------------------
int main() {
    experiment01();
    //experiment02();
    
}

//----------------------------------------------------------------------
// Create JSON objects from Person objects 
// Write them to file - Read from file.

void experiment01() {
    cout << "\nExperiment 01 - JSON Objects\n" << endl;

    // Create Person objects
    Person homer(100, "Homer Simpson", 39);
    Person marge(110, "Marge Simpson", 36);
    Person bart(120, "Bart Simpson", 10);

    // Serialize Person objects to JSON
    json homer_json = homer.to_json();
    json marge_json = marge.to_json();
    json bart_json = bart.to_json();

    // Print JSON representation (dump(4) indent 4 spaces)
    cout << "Homer JSON: " << homer_json.dump(4) << endl; 
    cout << "Marge JSON: " << marge_json.dump(4) << endl;
    cout << "Bart JSON: " << bart_json.dump(4) << endl;

    // Deserialize JSON to Person objects
    Person new_homer = Person::from_json(homer_json);
    Person new_marge = Person::from_json(marge_json);
    Person new_bart = Person::from_json(bart_json);

    // Print deserialized Person objects
    cout << "New Homer: " << new_homer.name << ", Age: " << new_homer.age << endl;
    cout << "New Marge: " << new_marge.name << ", Age: " << new_marge.age << endl;
    cout << "New Bart: " << new_bart.name << ", Age: " << new_bart.age << endl;


    //Optional - Manage the parts of a JSON object
    cout << "Homer's id:     " << homer_json["id"] << endl;
    cout << "Homer's name:   " << homer_json["name"] << endl;
    cout << "Homer's name:   " << homer_json["name"].get<string>() << endl;
    cout << "Homer's age:    " << homer_json["age"] << endl;
    cout << "Homer's age:    " << homer_json["age"].get<int>() << endl; //redundant get<int>()

}

//----------------------------------------------------------------------
// Create JSON list from Person objects
// Write them to file - Read from file.

void experiment02() {
    cout << "\nExperiment 02 - JSON Lists\n" << endl;

    //Make a list of Person objects
    Person list[3] = {
        Person(100, "Homer Simpson", 39),
        Person(110, "Marge Simpson", 36),
        Person(120, "Bart Simpson", 10),
    };

    //Serialize each person in the list
    //json behaves like a C++ vector
    json simpsons_json; 
    for (int i = 0; i < 3; i++) {
        simpsons_json.push_back(list[i].to_json());
    }

    //Print JSON representation of the list
    cout << simpsons_json.dump(4) << endl;

    //Deserialize - Make C++ list from simpsons_json
    Person newList[3];
    for (int i = 0; i < 3; i++) {
        newList[i] = Person::from_json(simpsons_json[i]);
        //show each person in the list
        cout << newList[i].toString() << endl;
    }

    //Save the entire list to a file in JSON format
    ofstream out_file("simpsons_list.json");
    // No endl needed, as dump(4) includes proper formatting
    out_file << simpsons_json.dump(4); 
    out_file.close();

    //Read the list from a file
    ifstream in_file("simpsons_list.json");
    json simpsons_list_json;
    in_file >> simpsons_list_json;
    in_file.close();

    //Show the deserialized list read from the file
    cout << "\nDeserialized JSON List from file\n";
 
    for (int i = 0; i < 3; i++) {
        Person dp = Person::from_json(simpsons_list_json[i]);
        //show each person in the list
        cout << dp.toString() << endl;
    }

}
//----------------------------------------------------------------------
void experiment03()
{
    cout << "\nExperiment 03 - JSON Objects\n" << endl;

    // Create Person objects
    Person homer(100, "Homer Simpson", 39);
    Person marge(110, "Marge Simpson", 36);
    Person bart(120, "Bart Simpson", 10);

    // Serialize Person objects to a JSON array
    json simpsons_json = json::array();
    simpsons_json.push_back(homer.to_json());
    simpsons_json.push_back(marge.to_json());
    simpsons_json.push_back(bart.to_json());

    // Serialize the JSON array to a file
    ofstream out_file("simpsons.json");
    out_file << simpsons_json.dump(4); // No endl needed, as dump(4) includes proper formatting
    out_file.close();

    // Read the entire file content into a string and then parse it as a JSON array.
    ifstream in_file("simpsons.json");
    stringstream buffer;
    buffer << in_file.rdbuf();
    string content = buffer.str();
    in_file.close();

    // Parse the content as a JSON array
    simpsons_json = json::parse(content);

    // Iterate over the JSON array and deserialize each JSON object
    for (const auto& item : simpsons_json) {
        Person dp = Person::from_json(item);
        cout << dp.toString() << endl;
    }
}