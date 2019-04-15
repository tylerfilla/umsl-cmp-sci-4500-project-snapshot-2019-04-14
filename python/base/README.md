# base

This is a Python module that provides (or *will* provide) base functionality:
- Command-line arguments
    - So the user can see help/version/legal info
    - So the user can override SQL server address/credentials
- Face detection
- Face recognition

#### Summary

Face detection and recognition are done off the Python thread, because CPython
has a global interpreter lock (GIL) that essentially prevents threads from being
put on different cores simultaneously. This isn't a problem for most workloads,
but, in Tyler's prototype, we saw empirical evidence of a problem, as the face
detection/recognition + the GIL put load on the Cozmo SDK loop. This could cause
delays in driving, camera frame retrieval, and overall responsiveness for users.

*We're doing our own face recognition, to begin with, because each Cozmo has a
built-in memory for ten faces, and these faces cannot be shared between two
Cozmos.*

We use the dlib C++ library and its accompanying data files to do face detection
using a histogram of gradients (HOG) method. Then we extract
personally-identifiable information from each detected face using dlib's face
"pose predictor" and face "recognizer" modules. This produces a 128-dimension
vector "encoding" (or "embedding", etc.) which is just an array/tuple of 64-bit
floats ("double"s in C/C++ and "float"s in Python).

We may try a more permissive face detector such as a Haar-based one from OpenCV.
