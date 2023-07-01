# API Status Page Design Document


# Introduction
This document outlines the design and development plan for a new API status page for our team. The goal is to create a user-friendly dashboard that provides real-time updates on the status of our APIs.


# Requirements
The following requirements must be met for the successful completion of this project:

* The status page must be written in HTML and JavaScript.
* It must query the API endpoint provided at http://api.braingenix.org/Diagnostic/Status using the appropriate HTTP methods (e.g., GET).
* The response from the API gateway for the status will be JSON formatted and contain the following attributes:  
    - (bgServiceStatus) `ServiceStateAPI=` Enum indicating the state of the API Service.  
    - (bgServiceStatus) `ServiceStateAPIDB=` Enum indicating the state of the API Database Service.  
    - (bgServiceStatus) `ServiceStateNES=` Enum indicating the state of the NES Service.  
    - (bgServiceStatus) `ServiceStateERS=` Enum indicating the state of the ERS Service.  
    - (bgServiceStatus) `ServiceStateSTS=` Enum indicating the state of the STS Service.  

* bgServiceStatus explanation:  
    - `0` Healthy  
    - `1` Degraded  
    - `2` Failed  
    - `3` Not Configured  

* If the gateway does not respond to http requests, or responds with a maformed request it should be assumed to be failed and show a failed status on all services. Additionally, a notice should be shown indicating the error so users may investigate for themselves if needed.    
* Status codes should match as follows: Healthy->Green, Degraded->Amber, Failed->Red, Not Configured->Grey  
* The status page must display the current status of each API endpoint in a clear and concise manner, using color coded boxes to display the state (e.g. Healthy, Degraded, Failed).
* The layout is not defined in this spec and may be designed as the writer sees fit. It should just follow the general format of other API status pages, and should use a dark color theme.

