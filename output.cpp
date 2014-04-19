#include "output.h"

Output::Output(std::string name2) {
  vertices = new std::vector<Point>();
  normals = new std::vector<Point>();
  faces = new std::vector<Point>();
  name = name2;
  index = 1;
}

void Output::exportFile() {
  std::ofstream myfile;
  myfile.open(name);

  myfile << "# Exported OBJ file.\n";

  myfile << "# List of vertices.\n";
  // Exporting the vertices
  for (int i = 0; i < vertices->size(); i++) {
    myfile << "v " << vertices->at(i).x() << " " << vertices->at(i).y() << " " << vertices->at(i).z() << "\n";
  }

  myfile << "\n";
  myfile << "# List of normals.\n";
  //Exporting the normals
  for (int i = 0; i < normals->size(); i++) {
    myfile << "vn " << normals->at(i).x() << " " << normals->at(i).y() << " " << normals->at(i).z() << "\n";
  }

  myfile << "\n";
  myfile << "# List of faces.\n";
  //Exporting the faces
  for (int i = 0; i < faces->size(); i++) {
    myfile << "f";
    for (int j = faces->at(i).x(); j <= faces->at(i).y(); j++) {
      myfile << " " << j << "//" << j;
    }
    myfile << "\n";
  }

  myfile.close();
  std::cout << "finished exporting to file" << std::endl;
}