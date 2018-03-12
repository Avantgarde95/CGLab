#include <iostream>
#include<algorithm>
#include<iterator>
#include "primitives.hpp"


void compute_normal_and_face(std::vector<glm::vec3>& point_vec, std::vector<glm::vec3>& normal_vec, std::vector<glm::vec3>& element_vec, int a, int b, int c)
{
	element_vec.emplace_back(glm::vec3(a, b, c));
	
	glm::vec3 first = point_vec[a];
	glm::vec3 second = point_vec[b];
	glm::vec3 third = point_vec[c];

	glm::vec3 f1 = second - first;
	glm::vec3 f2 = third - first;
	glm::vec3 f3 = glm::cross(f1,f2);

	element_vec[a] += f3;
	element_vec[b] += f3;
	element_vec[c] += f3;
}

void generate_sphere_coord(std::vector<glm::vec3>& point_vec, std::vector<glm::vec3>& normal_vec, std::vector<unsigned int>& element_vec, int level)
{
	const float X = .525731112119133606f;
	const float Z = .850650808352039932f;
	const float N = 0.f;

	struct Face
	{
		unsigned int a;
		unsigned int b;
		unsigned int c;
	};

	std::vector<glm::vec3> original_set = std::vector<glm::vec3>();
	std::vector<glm::vec3> faces = std::vector<glm::vec3>();

	original_set.emplace_back(glm::vec3(-X, N, Z));
	original_set.emplace_back(glm::vec3(X, N, Z));
	original_set.emplace_back(glm::vec3(-X, N, -Z));
	original_set.emplace_back(glm::vec3(X, N, -Z));
	original_set.emplace_back(glm::vec3(N,Z,X));
	original_set.emplace_back(glm::vec3(N, Z, -X));
	original_set.emplace_back(glm::vec3(N, -Z, X));
	original_set.emplace_back(glm::vec3(N, -Z, -X));
	original_set.emplace_back(glm::vec3(Z,X,N));
	original_set.emplace_back(glm::vec3(-Z, X, N));
	original_set.emplace_back(glm::vec3(Z, -X, N));
	original_set.emplace_back(glm::vec3(-Z, -X, N));

	faces.emplace_back(glm::vec3(0, 4, 1)); faces.emplace_back(glm::vec3(0, 9, 4)); faces.emplace_back(glm::vec3(9, 5, 4)); faces.emplace_back(glm::vec3(4, 5, 8)); faces.emplace_back(glm::vec3(4, 8, 1));
	faces.emplace_back(glm::vec3(8, 10, 1)); faces.emplace_back(glm::vec3(8, 3, 10)); faces.emplace_back(glm::vec3(5, 3, 8)); faces.emplace_back(glm::vec3(5, 2, 3)); faces.emplace_back(glm::vec3(2, 7, 3));
	faces.emplace_back(glm::vec3(7, 10, 3)); faces.emplace_back(glm::vec3(7, 6, 10)); faces.emplace_back(glm::vec3(7, 11, 6)); faces.emplace_back(glm::vec3(11, 0, 6)); faces.emplace_back(glm::vec3(0, 1, 6));
	faces.emplace_back(glm::vec3(6, 1, 10)); faces.emplace_back(glm::vec3(9, 0, 11)); faces.emplace_back(glm::vec3(9, 11, 2)); faces.emplace_back(glm::vec3(9, 2, 5)); faces.emplace_back(glm::vec3(7, 2, 11));
	
	for (int i = 0; i < original_set.size(); i++)
	{
		normal_vec.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	for (int i = 0; i < faces.size(); i++)
	{
		glm::vec3 a = original_set[(int)faces[i].z] - original_set[(int)faces[i].x];;
		glm::vec3 b = original_set[(int)faces[i].y] - original_set[(int)faces[i].x];;
		glm::vec3 n = glm::cross(b, a);

		normal_vec[(int)faces[i].x] += n;
		normal_vec[(int)faces[i].y] += n;
		normal_vec[(int)faces[i].z] += n;
	}

	for (int i = 0; i < level; i++)
	{
		std::vector<glm::vec3> temp_face = std::vector<glm::vec3>();
		temp_face.assign(faces.begin(), faces.end());
		
		faces.clear();

		for (int i = 0; i < temp_face.size(); i++)
		{
			int i0 = (int)temp_face[i].x;
			int i1 = (int)temp_face[i].y;
			int i2 = (int)temp_face[i].z;

			glm::vec3 v0 = original_set[i0]; 
			glm::vec3 v1 = original_set[i1];
			glm::vec3 v2 = original_set[i2];
			glm::vec3 v3 = glm::normalize((float)0.5 * (v0 + v1));
			glm::vec3 v4 = glm::normalize((float)0.5 * (v1 + v2));
			glm::vec3 v5 = glm::normalize((float)0.5 * (v2 + v0));

			original_set.emplace_back(v3);
			original_set.emplace_back(v4);
			original_set.emplace_back(v5);

			normal_vec.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
			normal_vec.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
			normal_vec.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));

			int current_index = original_set.size();
			int i3 = current_index;
			int i4 = current_index + 1;
			int i5 = current_index + 2;

			compute_normal_and_face(original_set, normal_vec, faces, i0, i3, i5);
			compute_normal_and_face(original_set, normal_vec, faces, i3, i1, i4);
			compute_normal_and_face(original_set, normal_vec, faces, i5, i4, i2);
			compute_normal_and_face(original_set, normal_vec, faces, i3, i4, i5);
		}
	}

	point_vec.assign(original_set.begin(), original_set.end());

	for (int i = 0; i < faces.size(); i++)
	{
		element_vec.emplace_back((unsigned int)faces[i].x);
		element_vec.emplace_back((unsigned int)faces[i].y);
		element_vec.emplace_back((unsigned int)faces[i].z);
	}
}
