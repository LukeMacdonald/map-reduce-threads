<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->

<a name="readme-top"></a>

<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url] [![MIT License][license-shield]][license-url] [![LinkedIn][linkedin-shield]][linkedin-url] <!-- PROJECT LOGO --> <br /> <div align="center"> <a href="https://github.com/LukeMacdonald/map-reduce-threads"> </a> <h3 align="center">MapReduce with C++ and Threads</h3> <p align="center"> <br />
<a href="https://github.com/LukeMacdonald/map-reduce-threads"><strong>Explore the docs »</strong></a>
<br />
<br />

  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

## About The Project

The C++ MapReduce Implementation project aims to explore various parallel processing techniques for executing MapReduce tasks under different circumstances. The project will sequentially implement MapReduce using processes, threads, threads with synchronization, and reading input from a stream. By comparing the performance and efficiency of each approach, the project seeks to provide insights into the strengths and limitations of different parallel processing paradigms

- MapReduce is a programming model and associated implementation for processing and generating large datasets in a distributed computing environment.
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

<div style="column-count: 2; -webkit-column-count: 2 -moz-column-count: 2;">

- [![C++][C++-badge]][C++-url]
- [![Ubuntu][Ubuntu-badge]][Ubuntu-url]

</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->

## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

### Prerequisites

- [C++ Compiler](https://code.visualstudio.com/docs/languages/cpp)

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/LukeMacdonald/map-reduce-threads.git
   ```
2. Compile Project
   ```sh
   make
   ```
3. Run individual tasks
   ```sh
   ./Task1 inputfile.txt outputfile.txt
   ./Task2 inputfile.txt outputfile.txt
   ./Task3 inputfile.txt outputfile.txt
   sudo ./Task4 inputfile.txt outputfile.txt
   ./Task5Server inputfile.txt | ./Task5 outputfile.txt
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Roadmap

- [x] Task 1: Clean up wordlists
  - [x] Remove duplicates
  - [x] Remove words with non-alphabetic characters
- [x] Task 2: Perform MapReduce with Processes
- [x] Task 3: Perform MapReduce with Threads
- [x] Task 4: Perform MapReduce with Synchronized Threads
- [x] Task 5: Perform MapReduce on Input from Stream

See the [open issues](https://github.com/LukeMacdonald/map-reduce-threads/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contact

Luke Macdonald - lukemacdonald21@gmail.com.com

Project Link: [https://github.com/LukeMacdonald/map-reduce-threads](https://github.com/LukeMacdonald/map-reduce-threads)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/LukeMacdonald/map-reduce-threads.svg?style=for-the-badge
[contributors-url]: https://github.com/LukeMacdonald/map-reduce-threads/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/LukeMacdonald/map-reduce-threads.svg?style=for-the-badge
[forks-url]: https://github.com/LukeMacdonald/map-reduce-threads/network/members
[stars-shield]: https://img.shields.io/github/stars/LukeMacdonald/map-reduce-threads.svg?style=for-the-badge
[stars-url]: https://github.com/LukeMacdonald/map-reduce-threads/stargazers
[issues-shield]: https://img.shields.io/github/issues/LukeMacdonald/map-reduce-threads.svg?style=for-the-badge
[issues-url]: https://github.com/LukeMacdonald/map-reduce-threads/issues
[license-shield]: https://img.shields.io/github/license/LukeMacdonald/map-reduce-threads.svg?style=for-the-badge
[license-url]: https://github.com/LukeMacdonald/map-reduce-threads/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/luke-macdonald-292a4a208
[product-screenshot]: images/screenshot.png
[C++-badge]: https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white
[C++-url]: https://cplusplus.com/doc/tutorial/
[Ubuntu-badge]: https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white
[Ubuntu-url]: https://ubuntu.com/
