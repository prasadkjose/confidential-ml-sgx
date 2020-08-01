[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/othneildrew/Best-README-Template">
    <img src="UI Design/icons/Encrypt.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Confidential Computing of Machine Learning using Intel SGX</h3>

  <p align="center">
    Master's Thesis
    <br />
    <br />
    <br />
    <a href="https://github.com/prasadkjose/Confidential_ML_SGX/tree/master/screen%20recordings">View Demo</a>
    ·
    <a href="https://github.com/prasadkjose/Confidential_ML_SGX/issues">Report Bug</a>
    
  </p>
</p>

<!-- TABLE OF CONTENTS -->

## Table of Contents

- [About the Project](#about-the-project)
  - [Built With](#built-with)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)

<!-- ABOUT THE PROJECT -->

## About The Project

<!-- [![Product Name Screen Shot][product-screenshot]](https://example.com) -->

A TEE-based application to securely upload and train multiparty data-sets with confidential computing.

[Thesis reprort](Master_Thesis.pdf)

### Built With

This platform was built with

- [c++](https://www.cplusplus.com)
- [WinAPI](https://docs.microsoft.com/en-us/windows/win32/)
- [Intel SGX](https://software.intel.com/content/www/us/en/develop/topics/software-guard-extensions.html)

<!-- GETTING STARTED -->

## Getting Started

Steps to run the software locally

### Prerequisites

- Intel SGX capable systems or Visual Studio 2017 with Intel SGX SDK
- Microsoft Windows 10 64-bit for Intel SGX support
- Intel processor that supports Intel
  Data Protection Technology(DPT) with Secure Key

### Installation

1. Install Visual Studio 2017 along with Intel SGX SDK
2. Clone the repo

```sh
git clone https://github.com/prasadkjose/Confidential_ML_SGX.git
```

3. Import the project in Visual Studio.
4. Run the application in debug mode or simutaion mode.

<!-- USAGE EXAMPLES -->

## Usage

There are two applications

- Encrypter

  - Open the Encrypter application
    <p align="center">
    <img src="Screenshots/EncrypterHome.png" alt="Logo" width="600" height="300"> 
    </p>

  - Login
     <p align="center">
    <img src="Screenshots/Login_enc.png" alt="Logo" width="600" height="300"> 
    </p>
  - Upload the Datasets
    <p align="center">
    <img src="Screenshots/EncrypterMain.png" alt="Logo" width="600" height="300"> 
    </p>

  - Once the files are uploaded, they can be viewed in the colsole and also listed under "Encrypteed Files"
    <p align="center">
      <img src="Screenshots/EncrypterUploaded.png" alt="Logo" width="600" height="300"> 
    </p>

* ML Uploader

  - Open the ML Uploader application.
    <p align="center">
      <img src="Screenshots/MLHome.png" alt="Logo" width="600" height="300">
    </p>
  - Log in and upload the datasets.
    <p align="center">
      <img src="Screenshots/MLUploaded.png" alt="Logo" width="600" height="300">
    </p>

  - Click Train to train the model
    <p align="center">
      <img src="Screenshots/MLTrain.png" alt="Logo" width="600" height="300">
    </p>

  <!-- MARKDOWN LINKS & IMAGES -->
  <!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=flat-square
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=flat-square
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=flat-square
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=flat-square
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=flat-square
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[encrypter1]: Screenshots/EncrypterHome.png
[thesis]: Master_Thesis.pdf
