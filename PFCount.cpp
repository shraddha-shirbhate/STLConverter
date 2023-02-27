#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <vector>
#include <list>
#include <unordered_set>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <cmath>

using namespace std;

class GeomVector
{
private:
    float x, y, z;
public:
    GeomVector(float x, float y, float z) : x{x}, y{y}, z{z} {};
    void set_x(float x)
    {
        x = x;
    }
    float get_x()
    {
        return x;
    }
    void set_y(float y)
    {
        y = y;
    }
    float get_y()
    {
        return y;
    }
    void set_z(float z)
    {
        z = z;
    }
    float get_z()
    {
        return z;
    }   
};

class STL_Reader
{
public:
    vector<GeomVector> readStlFile(string);
    void printAllVertices(vector<GeomVector>);
    map<string, int> uniqueVertices(vector<GeomVector>);
    void printUniqueVertices(map<string, int>);
    void facesPrint(map<string, int>, vector<GeomVector>);
};

vector<GeomVector> STL_Reader::readStlFile(string inputFile)
{
    vector<GeomVector> coordinates;

    int index = 0;
    ifstream ifs(inputFile, ios::in);
    if (ifs.good())
    {
        string line;
        string vertex;

        float x, y, z;

        while (getline(ifs, line))
        {
            stringstream ss;
            ss << line;
            ss >> vertex;
            if (vertex == "vertex")
            {
                ss >> x;
                ss >> y;
                ss >> z;
                index++;
                GeomVector g(x, y, z);
                coordinates.push_back(g);
            }
            else
                continue;
        }
        return coordinates;
    }
}

void STL_Reader::printAllVertices(vector<GeomVector> vertices)
{
    for (auto point : vertices)
    {
        cout << point.get_x() << " ";
        cout << point.get_y() << " ";
        cout << point.get_z() << endl;
    }
};

map<string, int> STL_Reader::uniqueVertices(vector<GeomVector> vertices)
{
    int index = 0;
    map<string, int> uniqueCoords;
    for (auto ver : vertices)
    {    
        string coords = to_string(ver.get_x()) + " " + to_string(ver.get_y()) + " " + to_string(ver.get_z()) + " ";
        if (uniqueCoords.find(coords) == uniqueCoords.end())
        {
            uniqueCoords.insert({coords, ++index});
        }
    }
    return uniqueCoords;
};

bool cmp(pair<string, int> &a, pair<string, int> &b)
{
    return a.second < b.second;
}

void STL_Reader::printUniqueVertices(map<string, int> printUniVertices)
{
    vector<pair<string, int>> A;
    for (auto &it : printUniVertices)
    {
        A.push_back(it);
    }
    sort(A.begin(), A.end(), cmp);

    for (auto it : A)
    {
        cout << "v " << it.first << " " << it.second << endl;
    }
};

void STL_Reader::facesPrint(map<string, int> printUniVertices, vector<GeomVector> vertices)
{
    int c = 0;
    for (auto ver : vertices)
    {
        if (c % 3 == 0)
            cout << "f ";
        string coords = to_string(ver.get_x()) + " " + to_string(ver.get_y()) + " " + to_string(ver.get_z()) + " ";
        cout << " " << printUniVertices.at(coords);
        c++;
        if (c % 3 == 0)
            cout << endl;
    }
}

int main()
{

    auto t0 = chrono::system_clock::now();

    /*INPUT FILE READING*/
    string inputFile = "cube_ASCII.stl";
    // string inputFile = "humanoid.stl";
    string outputFile = "cube_ASCII.obj";

    STL_Reader r;
    vector<GeomVector> vertices = r.readStlFile(inputFile);

    // cout<<"Print All Vertices : "<<endl;
    // f.printAllVertices(vertices);
    // cout<<"Number of All Vertices : "<< vertices.size() << endl;
    cout << endl;

    cout << "Number of triangles : " << vertices.size() / 3 << endl;
    map<string, int> uniquePoints = r.uniqueVertices(vertices);
    cout << "Count of Unique Vertices : " << uniquePoints.size() << endl;
    // cout << "Print Unique Vertices : " << endl;
    // f.printUniqueVertices(uniquePoints);
    cout << endl;
    // cout << "Print Faces : " << endl;
    // f.facesPrint(uniquePoints,vertices);

    auto t1 = chrono::system_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(t1 - t0);
    std::cout << "STL file is Read in Duration: " << elapsed.count() << " ms" << std::endl;

    /*OBJ FILE*/
    cout << "writing OBJ-File" << std::endl;
    ofstream fileOBJ(outputFile.c_str(), ios::out);
    fileOBJ << "# STL to OBJ Converter" << endl;
    fileOBJ << "# " << outputFile << endl;
    fileOBJ << endl;
    fileOBJ << "mtlib cube.mtl" << endl;

    map<string, int> OBJVertex = r.uniqueVertices(vertices);
    vector<pair<string, int>> A;
    for (auto &it : OBJVertex)
    {
        A.push_back(it);
    }
    sort(A.begin(), A.end(), cmp);

    for (auto it : A)
    {
        fileOBJ << "v " << it.first << endl;
    }
    fileOBJ << endl;

    auto t2 = chrono::system_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
    cout << "Unique Vertex is printed in OBJ File in : " << elapsed.count() << " ms" << std::endl;
    fileOBJ << "usemtl cube" << endl;

    /*Faces Count and writing into obj file*/
    vector<GeomVector> OBJAllVertices = r.readStlFile(inputFile);
    unsigned count = 0;
    for (auto ver : OBJAllVertices)
    {
        if (count % 3 == 0)
            fileOBJ << "f ";
        string coords = to_string(ver.get_x()) + " " + to_string(ver.get_y()) + " " + to_string(ver.get_z()) + " ";
        fileOBJ << " " << OBJVertex.at(coords);
        count++;
        if (count % 3 == 0)
            fileOBJ << endl;
    }

    auto t3 = chrono::system_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(t3 - t0);
    cout << "OBJ file written in " << elapsed.count() << " ms." << std::endl;

    return 0;
}