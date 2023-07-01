# Title
API Status Page Design Document

# Introduction
This document outlines the design and development plan for a new API status page for our team. The goal is to create a user-friendly dashboard that provides real-time updates on the status of our APIs.

# Requirements
The following requirements must be met for the successful completion of this project:

* The status page must be written in HTML and JavaScript.
* It must query the API endpoint provided at http://api.braingenix.org/Diagnostic/Status using the appropriate HTTP methods (e.g., GET).
* The response from the API gateway for the status will be JSON formatted and contain the following attributes:
 - 
* If the gateway does not respond to http requests, it should be assumed to be failed and show a failed status.
* Status codes should match as follows: Healthy->Green, Degraded->Amber, Failed->Red
* The status page must display the current status of each API endpoint in a clear and concise manner, using color coded boxes to display the state (e.g. Healthy, Degraded, Failed).
* The layout is not defined in this spec and may be designed as the writer sees fit. It should just follow the general format of other API status pages, and should use a dark color theme.