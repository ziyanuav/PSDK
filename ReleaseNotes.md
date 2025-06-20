# Ziyan Payload SDK Release Log

## V1.1.0.0-20250620-Release
**[Note]**:
* Introduced a new self-checking mechanism.
* Fixed several known issues.

**[Added]**:
* Added support for the self-checking mechanism. Users will now be prompted to update their PSPK version in the event of communication or version mismatches.
* Added support for the I2C communication protocol.

**[Optimized]**:
* Optimized the widget configuration file loading method. UAVs can now load configuration files dynamically.
* Optimized the speaker module to improve real-time speaker and TTS (text-to-speech) transmission logic.
* Refined the core initialization logic.

**[Fixed]**:
* Fixed an issue where the widget module occasionally failed to load the configuration file, causing the Ziyan app to not display payload widget information.
* Fixed an issue where log timestamps were displayed abnormally.
---




## V1.0.0.4-20250603-Release 
**[Note]**: 
* Added new module.
* Fixed some known issues.

**[Added]**:
* Add support for the speaker module.

**[Optimized]**:
* Optimize the core initialization process and provide more log information to support developers in quickly identifying the cause of exceptions during the adaptation process. 

**[Fixed]**:
* Fix issues that may cause abnormal program termination in specific communication scenarios.
* Fix the issue of strong coupling between the sample program and the system environment
---




## V1.0.0.3-20250429-Release 
**[Note]**: 
To support more camera expansion features, Add new camera API. Fixed some known issues.

**[Added]**:
* Adjust the feedback frequency of the camera status, support the remote control to refresh data at a higher frequency, and ensure real-time display.
* Add diverse debugging options

**[Fixed]**:
* Fix the dependency of the sample program and run the samples based on the computer system environment where they were compiled. 
---




## V1.0.0.2-20250401-Release 
**[Note]**: 
To support more camera expansion features, Add new camera API. Fixed some known issues.

**[Added]**:
* Add API which used to get obtain camera calculation position.
* Add API which used to control the camera tracker integrated into the payload camera.

**[Fixed]**:
* Fix the issue that the camera status is not updated when the camera has connected.
* Fix occasional screen distortion in camera videos
---




## V1.0.0.1-20250310-Release 
**[Note]**: 
To support more camera expansion features, Add new camera API. Fixed some known issues.

**[Added]**:
* Add API which used to control the range finder integrated into the payload camera.
---




## V1.0.0.0-20250306-Release 
**[Note]**: 
This version of Payload SDK mainly first release. Please refer to the release notes for detailed changes list.

**[Added]**:
* Added support for gimbal functionality
* Added support for basic camera functionality
* Added support for widget functionality
* Added support for subscription functionality
---






