
# Journal - student database

A project a utility that calculates the rating of students using different criteria


## Criteria

List of all criteria and their calculation formula:
| сriteria       | formula                | description |
| :-------------: |:------------------:| :-----:|
| attendance    | attendance/all classes    | student attendance|
| plus     | number of advantages/all classes |   received advantages | 
| evaluations  | total score for all the tests/5*total number tests          |    grades for the control|
| rating  | (attendance+pros+control)/3 |  average rating|

## Assembling

Installing qt library

```bash
$ apt-get update && apt-get install qt-default
```

Using cmake

```bash
$ cmake CMakeLists.txt
```

Using qmake

```bash
$ qmake journal.pro
$ make
```
## Usage from a command line

Add a student

```bash
$ student-rating add-student --course <course-name> --group <group-name> ---student-name <student-name>
```

Add a progress

```bash
$ student-rating add-progress --course <course-name> --group <group-name> ---student-name <student-name>
```

Show help message

```bash
$ student-rating --help
```










## Dependencies

**os version:** linux (5.x.x) or macos (10.x.x)

**qt version:** 5.x.x or higher

**cmake version:** 3.16.3 or higher

**qmake version:** 3.1.0 or higher


## Features

- Support for json/yaml data storage format
- Removing a student from a subject.
- Editing the student's name.
- Editing student attendance in the selected course


## License

[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg)](https://opensource.org/licenses/)
## Note

Project was created solely for educational purposes
