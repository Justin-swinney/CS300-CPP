// *** JUSTIN SWINNEY, CS300


#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <algorithm>

using namespace std;


class Course {
public:
	string courseNumber;
	string courseTitle;
	unordered_set<string> prerequisites;
};

// *** LOADING FILE ***

pair<unordered_map<string, Course>, unordered_set<string>> loadData(const string& filename) {
	unordered_map<string, Course> courses;
	unordered_set<string> courseNumbers;

	ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "ERROR: Failed to open input file" << endl;
		return make_pair(courses, courseNumbers);
	}

	string line;
	while (getline(file, line)) {
		Course course;
		stringstream ss(line);
		getline(ss, course.courseNumber, ',');
		getline(ss, course.courseTitle, ',');

		if (ss) {
			string prerequisite;
			while (getline(ss, prerequisite, ',')) {
				course.prerequisites.insert(prerequisite);
			}
		}

		if (course.courseNumber.empty() || course.courseTitle.empty()) {
			std::cout << "ERROR: Course cannont be added missing information." << endl;
		}
		else {
			courses.insert(make_pair(course.courseNumber, course));
			courseNumbers.insert(course.courseNumber);
		}
	}
	file.close();
	return make_pair(courses, courseNumbers);

}

// *** COMPARING COURSE TITLES FOR PROPER ALPHANUMERICAL FORMAT ***

struct CourseTitleCompare {
	bool operator()(const Course& x, const Course& y) const {
		if (x.courseNumber != y.courseNumber) {
			return x.courseNumber < y.courseNumber;
		}
		return x.courseTitle < y.courseTitle;
	}
};


 // *** PRINTING SEARCHED COURSE INFORMATION ***
void printCourseInfo(const Course& course) {
	std::cout << course.courseNumber << ", " << course.courseTitle << endl;

	if (!course.prerequisites.empty()) {
		std::cout << "Prerequisites: ";
		auto it = course.prerequisites.begin();
		while (it != course.prerequisites.end()) {
			std::cout << *it;
			++it;
			if (it != course.prerequisites.end()) {
				std::cout << ", ";
			}
		}
		std::cout << endl;
	}
}

// *** PRINTING ALPHANUMERICALLY ***

void printAlpha(const unordered_map<string, Course>& courses) {
	multimap<Course, string, CourseTitleCompare> sortedCourses;

	for (const auto& coursePair : courses) {
		sortedCourses.insert(make_pair(coursePair.second, coursePair.first));
	}

	for (const auto& sortedCourse : sortedCourses) {
		const Course& course = sortedCourse.first;
		std::cout << course.courseNumber << ", " << course.courseTitle << endl;

	}

}

// ** SEARCHING FOR COURSE BY THE COURSE NUMBER 

Course* searchCourseByNumber(unordered_map<string, Course>& courses, const string& courseNumber) {
	string searchlowerCase = courseNumber;
	transform(searchlowerCase.begin(), searchlowerCase.end(), searchlowerCase.begin(), tolower);

	auto it = courses.begin();
	while (it != courses.end()) {
		string courseLowerCase = it->first;
		transform(courseLowerCase.begin(), courseLowerCase.end(), courseLowerCase.begin(), tolower);
		if (courseLowerCase == searchlowerCase) {
			return &(it->second);
		}
		++it;
	}
	return nullptr;
}


int main() {
	pair<unordered_map<string, Course>, unordered_set<string>> courseInfo;
	unordered_map<string, Course>& courses = courseInfo.first;
	unordered_set<string>& courseNumbers = courseInfo.second;

	bool dataIsLoaded = false;
	int userChoice;

	std::cout << "Welcome to the course planner." << endl;
	

	while (true) {
		std::cout << endl;
		std::cout << "1. Load Data Structure." << endl;
		std::cout << "2. Print Course List." << endl;
		std::cout << "3. Print Course." << endl;
		std::cout << "4. Exit" << endl;

		std::cout << "What would you like to do?" << endl;
		std::cout << endl;
		cin >> userChoice;

		switch (userChoice) {
		case 1: {
			courseInfo = loadData("input_file.txt");
			courses = courseInfo.first;
			courseNumbers = courseInfo.second;
			dataIsLoaded = true;
			break;
		}
		case 2: {
			if (!dataIsLoaded) {
				std::cout << "Data must be loaded first" << endl;
				std::cout << endl;
			}
			else {
				std::cout << "Here is a sample schedule:" << endl;
				std::cout << endl;
				printAlpha(courses);
			}
			break;
		}
		case 3: {
			if (!dataIsLoaded) {
				std::cout << "Data must be loaded first" << endl;
				std::cout << endl;
			}
			else {
				string searchCourseNumber;
				std::cout << "What course do you want to know about?" << endl;
				cin >> searchCourseNumber;
				Course* foundCourse = searchCourseByNumber(courses, searchCourseNumber);
				if (foundCourse != nullptr) {
					printCourseInfo(*foundCourse);
				}
				else {
					std::cout << "Course not found." << endl;
				}
			}
			break;
		}

		case 4: {
			std::cout << "Thank you for using the course planner!" << endl;
			return 0;
		}
		default: 
			std::cout << userChoice << " is not a valid option." << endl;
			std::cout << endl;
		}
	}

}
