#if IS_FIBONACCI || HAS_COORDINATE_MAP

// Each Fibonacci board must define the following arrays:
//
// physicalToFibonacci[] === given physical pixel number, get it's order if sorted by distance from center (radius, aka the vogel spiral order)
// fibonacciToPhysical[] === given index to pixel in radial order (vogel spiral order), get physical pixel index
// angles[]              === angle, using units where 256 units == 360 degrees
// coordsX[] / coordsY[] === uint8_t coordinates for transformed spiral
//
// All boards appear to use the vogel model, with offset to first point == 0.
// [[ This can be confirmed by first element of angles[] === 0,
//    i.e., the first point is the center of the spiral at origin (x=0, y=0)
// ]]
//
// 1. Using radial coordinates, the Nth pixel's position is defined as:
// ```
// r = sqrt(n)
// theta = n * GOLDEN_ANGLE
// ```
//
// 2. Calculate all coordinates, giving Vogel's model centered at the origin
//
// 3. Store theta in `angles[]` (after converting to units where 256 units == 360 degrees)
//
// 4. Convert the radial coordinates to cartesian (x/y) coordinates
//
// 5. Transform the coordinates, such as via following psuedo-code:
//
// ```C++
// // first, translate from origin == center so left side and bottom aligned with X/Y axis
// double shiftX := min(originalCoordinatesX[]);
// double shiftY := min(originalCoordinatesY[]);
// double translatedX[] <== originalCoordinatesX[] - shiftX; // shifts right to align left side with X=0
// double translatedY[] = originalCoordinatesY[] - shiftY; // shifts upward to align bottom with Y=0
// // next, scale so that values lie entirely within the range [ 0.0 ... 256.0 ]
// double maxXY = max(max(translatedX[]), max(translatedY[]);
// double scaleFactor = 256.0 / maxXY;
// double scaledX[] = translatedX[] * scaleFactor;
// double scaledY[] = translatedY[] * scaleFactor;
// // finally, convert to integer values in the range [ 0 ... 255 ]
// uint8_t coordsX[] = (uint8_t)(scaledX[] - 0.5); // subtract 0.5 to avoid bias
// uint8_t coordsY[] = (uint8_t)(scaledY[] - 0.5); // subtract 0.5 to avoid bias
// ```
//
// There appears to have been a concious decision to store the X/Y
// values as 8-bit values.  Perhaps this was to allow for faster
// (8-bit) math operations on lower-power MCUs?
//
// Of course, the supported boards are ESP8266 or ESP32 (or SAMD21E),
// each of which are 32-bit native processors.  If this was on AVR,
// the continued use of 8-bit algorithms would be more critical.
//

#if defined(PRODUCT_FIBONACCI512)
  const uint16_t physicalToFibonacci[NUM_PIXELS] { 510, 476, 442, 408, 374, 340, 306, 272, 238, 204, 170, 136, 102, 68, 34, 0, 13, 47, 81, 115, 149, 183, 217, 251, 285, 319, 353, 387, 421, 455, 489, 502, 468, 434, 400, 366, 332, 298, 264, 230, 196, 162, 128, 94, 60, 26, 5, 39, 73, 107, 141, 175, 209, 243, 277, 311, 345, 379, 413, 447, 481, 494, 460, 426, 392, 358, 324, 290, 256, 222, 188, 154, 120, 86, 52, 18, 31, 65, 99, 133, 167, 201, 235, 269, 303, 337, 371, 405, 439, 473, 507, 486, 452, 418, 384, 350, 316, 282, 248, 214, 180, 146, 112, 78, 44, 10, 23, 57, 91, 125, 159, 193, 227, 261, 295, 329, 363, 397, 431, 465, 499, 478, 444, 410, 376, 342, 308, 274, 240, 206, 172, 138, 104, 70, 36, 2, 15, 49, 83, 117, 151, 185, 219, 253, 287, 321, 355, 389, 423, 457, 491, 504, 470, 436, 402, 368, 334, 300, 266, 232, 198, 164, 130, 96, 62, 28, 7, 41, 75, 109, 143, 177, 211, 245, 279, 313, 347, 381, 415, 449, 483, 496, 462, 428, 394, 360, 326, 292, 258, 224, 190, 156, 122, 88, 54, 20, 33, 67, 101, 135, 169, 203, 237, 271, 305, 339, 373, 407, 441, 475, 509, 488, 454, 420, 386, 352, 318, 284, 250, 216, 182, 148, 114, 80, 46, 12, 25, 59, 93, 127, 161, 195, 229, 263, 297, 331, 365, 399, 433, 467, 501, 480, 446, 412, 378, 344, 310, 276, 242, 208, 174, 140, 106, 72, 38, 4, 17, 51, 85, 119, 153, 187, 221, 255, 289, 323, 357, 391, 425, 459, 493, 506, 472, 438, 404, 370, 336, 302, 268, 234, 200, 166, 132, 98, 64, 30, 9, 43, 77, 111, 145, 179, 213, 247, 281, 315, 349, 383, 417, 451, 485, 498, 464, 430, 396, 362, 328, 294, 260, 226, 192, 158, 124, 90, 56, 22, 1, 35, 69, 103, 137, 171, 205, 239, 273, 307, 341, 375, 409, 443, 477, 511, 490, 456, 422, 388, 354, 320, 286, 252, 218, 184, 150, 116, 82, 48, 14, 27, 61, 95, 129, 163, 197, 231, 265, 299, 333, 367, 401, 435, 469, 503, 482, 448, 414, 380, 346, 312, 278, 244, 210, 176, 142, 108, 74, 40, 6, 19, 53, 87, 121, 155, 189, 223, 257, 291, 325, 359, 393, 427, 461, 495, 508, 474, 440, 406, 372, 338, 304, 270, 236, 202, 168, 134, 100, 66, 32, 11, 45, 79, 113, 147, 181, 215, 249, 283, 317, 351, 385, 419, 453, 487, 500, 466, 432, 398, 364, 330, 296, 262, 228, 194, 160, 126, 92, 58, 24, 3, 37, 71, 105, 139, 173, 207, 241, 275, 309, 343, 377, 411, 445, 479, 492, 458, 424, 390, 356, 322, 288, 254, 220, 186, 152, 118, 84, 50, 16, 29, 63, 97, 131, 165, 199, 233, 267, 301, 335, 369, 403, 437, 471, 505, 484, 450, 416, 382, 348, 314, 280, 246, 212, 178, 144, 110, 76, 42, 8, 21, 55, 89, 123, 157, 191, 225, 259, 293, 327, 361, 395, 429, 463, 497 };
  const uint16_t fibonacciToPhysical[NUM_PIXELS] { 15, 316, 135, 437, 255, 46, 376, 166, 496, 286, 105, 407, 225, 16, 346, 136, 466, 256, 75, 377, 195, 497, 315, 106, 436, 226, 45, 347, 165, 467, 285, 76, 406, 196, 14, 317, 134, 438, 254, 47, 375, 167, 495, 287, 104, 408, 224, 17, 345, 137, 465, 257, 74, 378, 194, 498, 314, 107, 435, 227, 44, 348, 164, 468, 284, 77, 405, 197, 13, 318, 133, 439, 253, 48, 374, 168, 494, 288, 103, 409, 223, 18, 344, 138, 464, 258, 73, 379, 193, 499, 313, 108, 434, 228, 43, 349, 163, 469, 283, 78, 404, 198, 12, 319, 132, 440, 252, 49, 373, 169, 493, 289, 102, 410, 222, 19, 343, 139, 463, 259, 72, 380, 192, 500, 312, 109, 433, 229, 42, 350, 162, 470, 282, 79, 403, 199, 11, 320, 131, 441, 251, 50, 372, 170, 492, 290, 101, 411, 221, 20, 342, 140, 462, 260, 71, 381, 191, 501, 311, 110, 432, 230, 41, 351, 161, 471, 281, 80, 402, 200, 10, 321, 130, 442, 250, 51, 371, 171, 491, 291, 100, 412, 220, 21, 341, 141, 461, 261, 70, 382, 190, 502, 310, 111, 431, 231, 40, 352, 160, 472, 280, 81, 401, 201, 9, 322, 129, 443, 249, 52, 370, 172, 490, 292, 99, 413, 219, 22, 340, 142, 460, 262, 69, 383, 189, 503, 309, 112, 430, 232, 39, 353, 159, 473, 279, 82, 400, 202, 8, 323, 128, 444, 248, 53, 369, 173, 489, 293, 98, 414, 218, 23, 339, 143, 459, 263, 68, 384, 188, 504, 308, 113, 429, 233, 38, 354, 158, 474, 278, 83, 399, 203, 7, 324, 127, 445, 247, 54, 368, 174, 488, 294, 97, 415, 217, 24, 338, 144, 458, 264, 67, 385, 187, 505, 307, 114, 428, 234, 37, 355, 157, 475, 277, 84, 398, 204, 6, 325, 126, 446, 246, 55, 367, 175, 487, 295, 96, 416, 216, 25, 337, 145, 457, 265, 66, 386, 186, 506, 306, 115, 427, 235, 36, 356, 156, 476, 276, 85, 397, 205, 5, 326, 125, 447, 245, 56, 366, 176, 486, 296, 95, 417, 215, 26, 336, 146, 456, 266, 65, 387, 185, 507, 305, 116, 426, 236, 35, 357, 155, 477, 275, 86, 396, 206, 4, 327, 124, 448, 244, 57, 365, 177, 485, 297, 94, 418, 214, 27, 335, 147, 455, 267, 64, 388, 184, 508, 304, 117, 425, 237, 34, 358, 154, 478, 274, 87, 395, 207, 3, 328, 123, 449, 243, 58, 364, 178, 484, 298, 93, 419, 213, 28, 334, 148, 454, 268, 63, 389, 183, 509, 303, 118, 424, 238, 33, 359, 153, 479, 273, 88, 394, 208, 2, 329, 122, 450, 242, 59, 363, 179, 483, 299, 92, 420, 212, 29, 333, 149, 453, 269, 62, 390, 182, 510, 302, 119, 423, 239, 32, 360, 152, 480, 272, 89, 393, 209, 1, 330, 121, 451, 241, 60, 362, 180, 482, 300, 91, 421, 211, 30, 332, 150, 452, 270, 61, 391, 181, 511, 301, 120, 422, 240, 31, 361, 151, 481, 271, 90, 392, 210, 0, 331 };
  const uint8_t  coordsX[NUM_PIXELS]             { 170, 178, 185, 191, 196, 199, 202, 202, 202, 200, 196, 191, 184, 175, 161, 132, 148, 166, 176, 183, 188, 190, 192, 191, 190, 186, 182, 176, 170, 162, 153, 125, 136, 145, 154, 161, 168, 173, 177, 180, 181, 181, 178, 174, 167, 155, 139, 157, 164, 169, 170, 170, 168, 165, 160, 154, 147, 139, 129, 119, 109, 82, 93, 104, 114, 124, 132, 140, 147, 152, 157, 159, 160, 159, 155, 145, 145, 150, 150, 149, 146, 141, 135, 127, 119, 110, 100, 90, 79, 68, 57, 45, 56, 66, 77, 87, 97, 107, 115, 123, 130, 135, 139, 142, 141, 136, 134, 134, 131, 126, 120, 113, 105, 96, 86, 76, 66, 56, 45, 35, 25, 18, 27, 37, 47, 57, 67, 77, 86, 95, 104, 112, 119, 124, 128, 129, 125, 120, 113, 105, 97, 88, 78, 69, 59, 50, 40, 31, 22, 14, 6, 0, 6, 12, 19, 27, 35, 44, 54, 63, 73, 82, 91, 101, 109, 118, 121, 108, 98, 88, 78, 69, 59, 50, 41, 33, 25, 18, 12, 7, 3, 6, 8, 11, 15, 20, 26, 33, 40, 48, 57, 67, 77, 87, 98, 111, 102, 89, 77, 67, 58, 49, 41, 34, 28, 23, 19, 16, 15, 14, 15, 26, 24, 23, 24, 26, 28, 33, 38, 44, 51, 60, 69, 80, 93, 111, 100, 85, 73, 64, 55, 49, 43, 39, 36, 34, 33, 34, 36, 39, 43, 58, 53, 49, 45, 44, 43, 44, 46, 50, 55, 61, 69, 79, 93, 116, 104, 87, 76, 68, 62, 58, 55, 54, 54, 55, 58, 62, 68, 74, 82, 108, 99, 90, 83, 77, 72, 68, 65, 64, 65, 67, 70, 76, 85, 97, 112, 94, 85, 80, 76, 75, 75, 77, 81, 85, 91, 98, 106, 115, 125, 153, 142, 131, 122, 113, 105, 99, 93, 89, 87, 85, 86, 89, 95, 106, 123, 105, 99, 96, 96, 98, 101, 106, 112, 119, 128, 137, 147, 157, 168, 180, 193, 182, 171, 161, 151, 141, 133, 125, 118, 113, 108, 106, 106, 108, 115, 116, 114, 115, 118, 123, 129, 137, 145, 154, 164, 174, 184, 195, 206, 216, 226, 216, 205, 195, 185, 175, 165, 156, 147, 139, 133, 127, 123, 121, 124, 127, 129, 134, 141, 148, 156, 165, 174, 184, 194, 204, 214, 224, 233, 242, 253, 246, 238, 229, 221, 211, 202, 192, 182, 173, 164, 155, 147, 140, 133, 134, 142, 151, 160, 169, 179, 188, 198, 207, 216, 225, 233, 241, 247, 253, 255, 251, 247, 241, 234, 227, 219, 210, 201, 192, 182, 173, 163, 153, 142, 134, 151, 163, 174, 184, 194, 203, 212, 220, 227, 233, 239, 243, 247, 249, 242, 242, 241, 238, 235, 230, 225, 218, 211, 203, 194, 184, 173, 161, 146, 155, 170, 182, 192, 201, 208, 215, 221, 226, 229, 231, 232, 232, 231, 228, 215, 219, 221, 223, 223, 221, 219, 215, 210, 204, 197, 188, 177, 164, 144, 154, 170, 182, 191, 198, 204, 208, 211, 212, 213, 212, 209, 205, 200, 194 };
  const uint8_t  coordsY[NUM_PIXELS]             { 6, 14, 22, 31, 41, 50, 60, 70, 80, 89, 98, 106, 114, 120, 125, 128, 124, 116, 109, 101, 92, 82, 73, 63, 54, 44, 35, 27, 18, 11, 4, 0, 5, 10, 17, 24, 32, 40, 49, 59, 68, 78, 87, 97, 106, 116, 121, 106, 95, 85, 75, 65, 56, 47, 38, 31, 24, 17, 12, 7, 4, 10, 10, 13, 16, 20, 25, 31, 39, 46, 55, 64, 74, 85, 96, 111, 101, 87, 76, 65, 56, 48, 41, 34, 29, 25, 21, 20, 19, 19, 21, 33, 30, 29, 28, 29, 31, 34, 39, 45, 51, 59, 69, 79, 92, 111, 101, 85, 73, 64, 56, 50, 45, 41, 39, 38, 38, 40, 43, 47, 52, 68, 62, 56, 53, 50, 49, 49, 50, 53, 57, 63, 70, 80, 94, 119, 106, 89, 78, 70, 65, 61, 59, 59, 60, 62, 66, 71, 77, 84, 92, 119, 109, 100, 92, 85, 80, 75, 72, 70, 70, 71, 74, 79, 87, 99, 114, 97, 88, 83, 81, 80, 81, 84, 88, 94, 100, 108, 116, 126, 136, 163, 152, 142, 132, 123, 114, 107, 101, 96, 93, 91, 91, 93, 98, 108, 108, 102, 101, 101, 104, 108, 114, 120, 128, 137, 146, 157, 167, 178, 190, 203, 192, 181, 170, 160, 150, 141, 133, 126, 119, 115, 111, 110, 111, 118, 119, 118, 120, 124, 130, 136, 144, 153, 162, 172, 182, 193, 203, 214, 224, 232, 223, 213, 202, 192, 182, 172, 163, 154, 146, 139, 132, 128, 125, 126, 129, 133, 139, 146, 154, 162, 172, 181, 191, 201, 210, 220, 229, 238, 246, 255, 249, 242, 234, 225, 216, 207, 197, 188, 178, 169, 160, 151, 143, 136, 135, 145, 155, 164, 174, 183, 193, 202, 211, 220, 228, 236, 243, 249, 254, 253, 250, 246, 242, 236, 229, 222, 213, 205, 196, 186, 176, 166, 155, 143, 133, 153, 166, 177, 187, 196, 205, 214, 221, 228, 234, 238, 242, 245, 246, 246, 236, 237, 237, 236, 233, 229, 225, 219, 212, 204, 195, 186, 175, 162, 146, 156, 171, 182, 192, 201, 208, 214, 220, 223, 226, 228, 228, 227, 224, 221, 207, 211, 215, 217, 218, 218, 216, 213, 209, 203, 196, 187, 177, 164, 142, 153, 170, 181, 190, 196, 201, 205, 207, 208, 207, 205, 202, 197, 191, 184, 159, 168, 176, 182, 188, 192, 195, 197, 197, 196, 193, 189, 182, 173, 160, 146, 164, 173, 180, 184, 186, 186, 185, 183, 179, 174, 167, 160, 151, 142, 114, 125, 135, 144, 152, 159, 165, 170, 173, 175, 176, 174, 171, 164, 153, 136, 154, 161, 164, 165, 164, 162, 157, 152, 145, 138, 129, 119, 109, 98, 72, 83, 94, 105, 114, 124, 132, 139, 145, 150, 153, 155, 155, 152, 143, 142, 146, 146, 143, 139, 134, 127, 119, 110, 101, 91, 81, 70, 59, 48, 37, 48, 58, 69, 79, 89, 99, 108, 116, 123, 129, 134, 137, 138, 134, 132, 130, 126, 121, 114, 106, 98, 88, 79, 69, 59, 48, 38, 29, 19 };
  const uint8_t  angles[NUM_PIXELS]              { 205, 208, 211, 215, 218, 221, 225, 228, 231, 235, 238, 242, 245, 248, 252, 255, 246, 243, 239, 236, 233, 229, 226, 223, 219, 216, 213, 209, 206, 203, 199, 190, 194, 197, 200, 204, 207, 211, 214, 217, 221, 224, 227, 231, 234, 237, 232, 229, 225, 222, 219, 215, 212, 208, 205, 202, 198, 195, 192, 188, 185, 176, 180, 183, 186, 190, 193, 196, 200, 203, 206, 210, 213, 216, 220, 223, 214, 211, 208, 204, 201, 198, 194, 191, 188, 184, 181, 177, 174, 171, 167, 162, 165, 169, 172, 175, 179, 182, 185, 189, 192, 196, 199, 202, 206, 209, 200, 197, 193, 190, 187, 183, 180, 177, 173, 170, 167, 163, 160, 157, 153, 148, 151, 154, 158, 161, 165, 168, 171, 175, 178, 181, 185, 188, 191, 195, 186, 183, 179, 176, 173, 169, 166, 162, 159, 156, 152, 149, 146, 142, 139, 130, 133, 137, 140, 144, 147, 150, 154, 157, 160, 164, 167, 170, 174, 177, 172, 168, 165, 162, 158, 155, 152, 148, 145, 141, 138, 135, 131, 128, 125, 116, 119, 123, 126, 129, 133, 136, 139, 143, 146, 149, 153, 156, 160, 163, 154, 151, 147, 144, 141, 137, 134, 131, 127, 124, 121, 117, 114, 110, 107, 102, 105, 108, 112, 115, 118, 122, 125, 129, 132, 135, 139, 142, 145, 149, 140, 137, 133, 130, 126, 123, 120, 116, 113, 110, 106, 103, 100, 96, 93, 87, 91, 94, 98, 101, 104, 108, 111, 114, 118, 121, 124, 128, 131, 134, 126, 122, 119, 116, 112, 109, 106, 102, 99, 95, 92, 89, 85, 82, 79, 70, 73, 77, 80, 83, 87, 90, 93, 97, 100, 103, 107, 110, 114, 117, 111, 108, 105, 101, 98, 95, 91, 88, 85, 81, 78, 74, 71, 68, 64, 56, 59, 62, 66, 69, 72, 76, 79, 82, 86, 89, 93, 96, 99, 103, 97, 94, 90, 87, 84, 80, 77, 74, 70, 67, 64, 60, 57, 54, 50, 47, 41, 45, 48, 51, 55, 58, 62, 65, 68, 72, 75, 78, 82, 85, 88, 80, 76, 73, 70, 66, 63, 59, 56, 53, 49, 46, 43, 39, 36, 33, 27, 31, 34, 37, 41, 44, 47, 51, 54, 57, 61, 64, 67, 71, 74, 65, 62, 59, 55, 52, 49, 45, 42, 39, 35, 32, 28, 25, 22, 18, 10, 13, 16, 20, 23, 26, 30, 33, 36, 40, 43, 47, 50, 53, 57, 51, 48, 44, 41, 38, 34, 31, 28, 24, 21, 18, 14, 11, 8, 4, 251, 254, 2, 5, 9, 12, 16, 19, 22, 26, 29, 32, 36, 39, 42, 37, 34, 30, 27, 24, 20, 17, 13, 10, 7, 3, 0, 252, 249, 245, 236, 240, 243, 247, 250, 253, 1, 5, 8, 11, 15, 18, 21, 25, 28, 19, 16, 13, 9, 6, 3, 255, 251, 248, 244, 241, 238, 234, 231, 228, 222, 226, 229, 232, 236, 239, 242, 246, 249, 252, 0, 4, 7, 11, 14, 5, 2, 254, 250, 247, 244, 240, 237, 234, 230, 227, 223, 220, 217, 213 };
#elif defined(PRODUCT_FIBONACCI256)
  const uint8_t  physicalToFibonacci[NUM_PIXELS] { 0, 13, 26, 39, 52, 65, 78, 91, 104, 117, 130, 143, 156, 169, 182, 195, 208, 221, 234, 247, 252, 239, 226, 213, 200, 187, 174, 161, 148, 135, 122, 109, 96, 83, 70, 57, 44, 31, 18, 5, 10, 23, 36, 49, 62, 75, 88, 101, 114, 127, 140, 153, 166, 179, 192, 205, 218, 231, 244, 249, 236, 223, 210, 197, 184, 171, 158, 145, 132, 119, 106, 93, 80, 67, 54, 41, 28, 15, 2, 7, 20, 33, 46, 59, 72, 85, 98, 111, 124, 137, 150, 163, 176, 189, 202, 215, 228, 241, 254, 246, 233, 220, 207, 194, 181, 168, 155, 142, 129, 116, 103, 90, 77, 64, 51, 38, 25, 12, 4, 17, 30, 43, 56, 69, 82, 95, 108, 121, 134, 147, 160, 173, 186, 199, 212, 225, 238, 251, 243, 230, 217, 204, 191, 178, 165, 152, 139, 126, 113, 100, 87, 74, 61, 48, 35, 22, 9, 1, 14, 27, 40, 53, 66, 79, 92, 105, 118, 131, 144, 157, 170, 183, 196, 209, 222, 235, 248, 253, 240, 227, 214, 201, 188, 175, 162, 149, 136, 123, 110, 97, 84, 71, 58, 45, 32, 19, 6, 11, 24, 37, 50, 63, 76, 89, 102, 115, 128, 141, 154, 167, 180, 193, 206, 219, 232, 245, 250, 237, 224, 211, 198, 185, 172, 159, 146, 133, 120, 107, 94, 81, 68, 55, 42, 29, 16, 3, 8, 21, 34, 47, 60, 73, 86, 99, 112, 125, 138, 151, 164, 177, 190, 203, 216, 229, 242, 255 };
  const uint8_t  fibonacciToPhysical[NUM_PIXELS] { 0, 157, 78, 235, 118, 39, 196, 79, 236, 156, 40, 197, 117, 1, 158, 77, 234, 119, 38, 195, 80, 237, 155, 41, 198, 116, 2, 159, 76, 233, 120, 37, 194, 81, 238, 154, 42, 199, 115, 3, 160, 75, 232, 121, 36, 193, 82, 239, 153, 43, 200, 114, 4, 161, 74, 231, 122, 35, 192, 83, 240, 152, 44, 201, 113, 5, 162, 73, 230, 123, 34, 191, 84, 241, 151, 45, 202, 112, 6, 163, 72, 229, 124, 33, 190, 85, 242, 150, 46, 203, 111, 7, 164, 71, 228, 125, 32, 189, 86, 243, 149, 47, 204, 110, 8, 165, 70, 227, 126, 31, 188, 87, 244, 148, 48, 205, 109, 9, 166, 69, 226, 127, 30, 187, 88, 245, 147, 49, 206, 108, 10, 167, 68, 225, 128, 29, 186, 89, 246, 146, 50, 207, 107, 11, 168, 67, 224, 129, 28, 185, 90, 247, 145, 51, 208, 106, 12, 169, 66, 223, 130, 27, 184, 91, 248, 144, 52, 209, 105, 13, 170, 65, 222, 131, 26, 183, 92, 249, 143, 53, 210, 104, 14, 171, 64, 221, 132, 25, 182, 93, 250, 142, 54, 211, 103, 15, 172, 63, 220, 133, 24, 181, 94, 251, 141, 55, 212, 102, 16, 173, 62, 219, 134, 23, 180, 95, 252, 140, 56, 213, 101, 17, 174, 61, 218, 135, 22, 179, 96, 253, 139, 57, 214, 100, 18, 175, 60, 217, 136, 21, 178, 97, 254, 138, 58, 215, 99, 19, 176, 59, 216, 137, 20, 177, 98, 255 };
  const uint8_t  coordsX[NUM_PIXELS]             { 133, 156, 165, 168, 165, 158, 147, 132, 114, 95, 76, 57, 41, 28, 19, 15, 17, 24, 37, 56, 123, 96, 73, 53, 38, 28, 24, 25, 31, 41, 55, 71, 89, 106, 122, 136, 146, 152, 152, 143, 138, 136, 128, 115, 101, 85, 70, 56, 44, 37, 33, 34, 41, 53, 69, 90, 114, 140, 167, 226, 204, 180, 154, 129, 106, 85, 67, 54, 46, 43, 44, 50, 60, 72, 86, 100, 113, 123, 128, 117, 104, 90, 78, 67, 59, 54, 54, 59, 68, 82, 100, 121, 143, 167, 191, 212, 231, 246, 255, 251, 251, 245, 233, 218, 199, 178, 156, 134, 114, 96, 82, 73, 67, 66, 70, 78, 89, 103, 111, 94, 84, 80, 81, 86, 96, 109, 126, 145, 165, 185, 204, 220, 233, 241, 244, 241, 232, 217, 179, 201, 217, 229, 235, 235, 230, 220, 207, 190, 172, 154, 136, 121, 108, 99, 95, 96, 104, 120, 110, 111, 118, 130, 144, 160, 176, 192, 206, 217, 224, 227, 224, 216, 202, 184, 162, 137, 110, 44, 68, 94, 120, 145, 168, 187, 202, 212, 216, 216, 212, 203, 191, 177, 162, 148, 135, 126, 122, 136, 147, 161, 174, 186, 197, 204, 206, 205, 198, 187, 172, 152, 130, 106, 81, 58, 36, 17, 0, 5, 15, 30, 49, 71, 93, 116, 138, 157, 173, 185, 192, 195, 193, 187, 178, 166, 152, 137, 149, 164, 175, 180, 182, 179, 171, 159, 143, 125, 105, 83, 63, 44, 28, 16, 9, 7, 12, 23 };
  const uint8_t  coordsY[NUM_PIXELS]             { 126, 120, 109, 96, 82, 69, 57, 49, 45, 45, 50, 59, 74, 92, 114, 138, 163, 188, 211, 231, 255, 248, 235, 218, 198, 175, 152, 129, 107, 89, 74, 63, 57, 56, 59, 66, 76, 88, 102, 116, 103, 88, 77, 71, 68, 70, 77, 88, 103, 121, 141, 163, 184, 205, 222, 236, 245, 249, 247, 208, 224, 235, 241, 240, 234, 223, 209, 191, 172, 152, 132, 115, 101, 90, 84, 82, 86, 95, 114, 107, 98, 98, 103, 112, 126, 142, 159, 177, 195, 210, 222, 230, 233, 230, 223, 209, 191, 168, 142, 98, 125, 151, 174, 194, 209, 219, 223, 223, 218, 208, 195, 180, 164, 148, 134, 122, 114, 112, 123, 128, 138, 151, 165, 180, 193, 203, 211, 214, 212, 206, 194, 178, 158, 134, 109, 83, 58, 35, 11, 28, 48, 71, 95, 120, 142, 163, 179, 192, 200, 203, 202, 196, 187, 175, 162, 148, 136, 133, 152, 166, 177, 186, 190, 191, 187, 178, 165, 148, 128, 107, 84, 62, 41, 24, 11, 2, 0, 28, 16, 9, 8, 13, 23, 37, 55, 75, 96, 116, 135, 151, 164, 173, 177, 177, 172, 162, 146, 153, 161, 163, 160, 152, 139, 124, 106, 87, 69, 51, 36, 25, 18, 16, 20, 29, 44, 64, 133, 106, 81, 60, 44, 32, 26, 25, 29, 38, 50, 65, 82, 99, 115, 129, 140, 147, 148, 138, 134, 131, 122, 110, 95, 80, 65, 52, 42, 36, 34, 37, 45, 59, 77, 98, 123, 149, 176, 202 };
  const uint8_t  angles[NUM_PIXELS]              { 0, 247, 238, 229, 220, 211, 203, 194, 185, 176, 167, 159, 150, 141, 132, 123, 115, 106, 97, 88, 65, 74, 83, 92, 100, 109, 118, 127, 136, 144, 153, 162, 171, 180, 188, 197, 206, 215, 224, 232, 209, 201, 192, 183, 174, 165, 157, 148, 139, 130, 121, 113, 104, 95, 86, 77, 69, 60, 51, 28, 37, 46, 54, 63, 72, 81, 90, 98, 107, 116, 125, 134, 142, 151, 160, 169, 178, 186, 195, 172, 163, 155, 146, 137, 128, 119, 111, 102, 93, 84, 75, 67, 58, 49, 40, 31, 23, 14, 5, 246, 255, 8, 17, 26, 35, 44, 52, 61, 70, 79, 88, 96, 105, 114, 123, 132, 140, 149, 135, 126, 117, 108, 100, 91, 82, 73, 64, 56, 47, 38, 29, 20, 12, 3, 250, 241, 232, 223, 209, 218, 227, 235, 244, 253, 6, 15, 24, 33, 41, 50, 59, 68, 77, 85, 94, 103, 112, 98, 89, 80, 71, 62, 54, 45, 36, 27, 18, 10, 1, 247, 239, 230, 221, 212, 203, 195, 186, 163, 172, 180, 189, 198, 207, 216, 224, 233, 242, 251, 4, 13, 22, 31, 39, 48, 57, 66, 75, 52, 43, 34, 25, 16, 8, 254, 245, 237, 228, 219, 210, 201, 193, 184, 175, 166, 157, 149, 126, 134, 143, 152, 161, 170, 178, 187, 196, 205, 214, 222, 231, 240, 249, 2, 11, 20, 28, 37, 14, 5, 252, 243, 235, 226, 217, 208, 199, 191, 182, 173, 164, 155, 147, 138, 129, 120, 111, 103 };
#elif defined(PRODUCT_FIBONACCI128)
  const uint8_t  physicalToFibonacci[NUM_PIXELS] { 0, 13, 26, 39, 52, 65, 78, 91, 104, 117, 122, 109, 96, 83, 70, 57, 44, 31, 18, 5, 10, 23, 36, 49, 62, 75, 88, 101, 114, 127, 119, 106, 93, 80, 67, 54, 41, 28, 15, 2, 7, 20, 33, 46, 59, 72, 85, 98, 111, 124, 116, 103, 90, 77, 64, 51, 38, 25, 12, 4, 17, 30, 43, 56, 69, 82, 95, 108, 121, 126, 113, 100, 87, 74, 61, 48, 35, 22, 9, 1, 14, 27, 40, 53, 66, 79, 92, 105, 118, 123, 110, 97, 84, 71, 58, 45, 32, 19, 6, 11, 24, 37, 50, 63, 76, 89, 102, 115, 120, 107, 94, 81, 68, 55, 42, 29, 16, 3, 8, 21, 34, 47, 60, 73, 86, 99, 112, 125 };
  const uint8_t  fibonacciToPhysical[NUM_PIXELS] { 0, 79, 39, 117, 59, 19, 98, 40, 118, 78, 20, 99, 58, 1, 80, 38, 116, 60, 18, 97, 41, 119, 77, 21, 100, 57, 2, 81, 37, 115, 61, 17, 96, 42, 120, 76, 22, 101, 56, 3, 82, 36, 114, 62, 16, 95, 43, 121, 75, 23, 102, 55, 4, 83, 35, 113, 63, 15, 94, 44, 122, 74, 24, 103, 54, 5, 84, 34, 112, 64, 14, 93, 45, 123, 73, 25, 104, 53, 6, 85, 33, 111, 65, 13, 92, 46, 124, 72, 26, 105, 52, 7, 86, 32, 110, 66, 12, 91, 47, 125, 71, 27, 106, 51, 8, 87, 31, 109, 67, 11, 90, 48, 126, 70, 28, 107, 50, 9, 88, 30, 108, 68, 10, 89, 49, 127, 69, 29 };
  const uint8_t  coordsX[NUM_PIXELS]             { 137, 170, 182, 186, 182, 172, 156, 135, 110, 83, 26, 49, 74, 99, 121, 141, 155, 164, 164, 151, 145, 141, 129, 112, 91, 69, 47, 27, 11, 0, 9, 11, 19, 33, 50, 70, 90, 108, 123, 130, 114, 96, 77, 58, 43, 32, 25, 25, 32, 45, 85, 65, 51, 43, 42, 47, 58, 74, 94, 105, 81, 68, 62, 63, 70, 84, 103, 127, 154, 218, 193, 167, 142, 119, 101, 89, 83, 84, 96, 118, 104, 106, 116, 132, 152, 175, 198, 221, 241, 255, 248, 236, 219, 199, 178, 158, 140, 126, 121, 140, 157, 176, 195, 213, 227, 237, 241, 239, 193, 210, 221, 225, 222, 214, 201, 184, 164, 142, 160, 181, 196, 204, 206, 202, 191, 174, 152, 125 };
  const uint8_t  coordsY[NUM_PIXELS]             { 130, 121, 105, 86, 66, 47, 31, 19, 13, 13, 55, 39, 30, 28, 33, 43, 57, 75, 95, 115, 96, 75, 60, 50, 46, 49, 59, 74, 96, 122, 166, 139, 114, 93, 78, 69, 67, 72, 85, 112, 102, 90, 89, 96, 110, 129, 152, 177, 203, 227, 247, 228, 207, 184, 161, 140, 123, 112, 109, 126, 132, 146, 165, 185, 206, 225, 240, 251, 255, 224, 235, 240, 237, 229, 216, 199, 181, 161, 143, 139, 166, 186, 202, 215, 221, 222, 216, 204, 185, 115, 142, 166, 184, 197, 203, 202, 195, 181, 158, 167, 180, 182, 178, 166, 148, 126, 101, 74, 21, 42, 66, 91, 114, 134, 150, 159, 160, 147, 141, 137, 124, 106, 85, 63, 42, 24, 9, 0 };
  const uint8_t  angles[NUM_PIXELS]              { 0, 247, 239, 230, 221, 212, 203, 194, 186, 177, 154, 163, 171, 180, 189, 198, 207, 216, 224, 233, 210, 201, 192, 184, 175, 166, 157, 148, 139, 131, 116, 125, 134, 143, 152, 160, 169, 178, 187, 196, 173, 164, 155, 146, 137, 129, 120, 111, 102, 93, 79, 88, 97, 105, 114, 123, 132, 141, 150, 135, 126, 118, 109, 100, 91, 82, 73, 65, 56, 33, 42, 50, 59, 68, 77, 86, 95, 103, 112, 98, 89, 80, 71, 63, 54, 45, 36, 27, 18, 252, 4, 13, 22, 31, 39, 48, 57, 66, 75, 52, 43, 34, 25, 16, 8, 255, 246, 237, 214, 223, 232, 241, 250, 2, 11, 20, 29, 37, 14, 5, 253, 244, 235, 226, 218, 209, 200, 191 };
#elif defined(PRODUCT_FIBONACCI64_FULL) || defined(PRODUCT_FIBONACCI64_MINI)
  const uint8_t  physicalToFibonacci[NUM_PIXELS] { 0, 13, 26, 39, 52, 57, 44, 31, 18, 5, 10, 23, 36, 49, 62, 54, 41, 28, 15, 2, 7, 20, 33, 46, 59, 51, 38, 25, 12, 4, 17, 30, 43, 56, 61, 48, 35, 22, 9, 1, 14, 27, 40, 53, 58, 45, 32, 19, 6, 11, 24, 37, 50, 63, 55, 42, 29, 16, 3, 8, 21, 34, 47, 60 };
  const uint8_t  fibonacciToPhysical[NUM_PIXELS] { 0, 39, 19, 58, 29, 9, 48, 20, 59, 38, 10, 49, 28, 1, 40, 18, 57, 30, 8, 47, 21, 60, 37, 11, 50, 27, 2, 41, 17, 56, 31, 7, 46, 22, 61, 36, 12, 51, 26, 3, 42, 16, 55, 32, 6, 45, 23, 62, 35, 13, 52, 25, 4, 43, 15, 54, 33, 5, 44, 24, 63, 34, 14, 53 };
  const uint8_t  coordsX[NUM_PIXELS]             { 140, 189, 208, 214, 208, 146, 168, 180, 180, 162, 152, 146, 129, 103, 72, 40, 70, 97, 120, 131, 107, 79, 50, 23, 0, 7, 23, 46, 76, 93, 57, 37, 28, 29, 87, 68, 59, 62, 80, 113, 91, 94, 109, 133, 202, 172, 145, 125, 117, 145, 170, 198, 227, 253, 255, 235, 210, 181, 148, 175, 207, 228, 240, 244 };
  const uint8_t  coordsY[NUM_PIXELS]             { 128, 114, 91, 63, 34, 0, 21, 48, 76, 106, 78, 47, 25, 11, 5, 38, 35, 42, 61, 101, 87, 69, 68, 78, 98, 143, 118, 102, 98, 122, 131, 152, 179, 209, 255, 230, 202, 174, 148, 142, 181, 210, 235, 252, 235, 234, 224, 203, 170, 183, 201, 205, 198, 181, 134, 157, 171, 173, 153, 145, 138, 120, 93, 63 };
  const uint8_t  angles[NUM_PIXELS]              { 0, 249, 241, 232, 223, 200, 208, 217, 226, 235, 212, 203, 194, 185, 176, 162, 171, 180, 188, 197, 174, 165, 156, 147, 139, 124, 133, 142, 151, 136, 128, 119, 110, 101, 78, 86, 95, 104, 113, 99, 90, 81, 72, 63, 40, 49, 58, 67, 75, 52, 43, 34, 25, 17, 2, 11, 20, 29, 38, 14, 6, 255, 246, 237 };
#elif defined(PRODUCT_FIBONACCI32)
  const uint8_t  physicalToFibonacci[NUM_PIXELS] { 0, 13, 26, 31, 18, 5, 10, 23, 28, 15, 2, 7, 20, 33, 25, 12, 4, 17, 30, 22, 9, 1, 14, 27, 32, 19, 6, 11, 24, 29, 16, 3, 8, 21 };
  const uint8_t  fibonacciToPhysical[NUM_PIXELS] { 0, 21, 10, 31, 16, 5, 26, 11, 32, 20, 6, 27, 15, 1, 22, 9, 30, 17, 4, 25, 12, 33, 19, 7, 28, 14, 2, 23, 8, 29, 18, 3, 24, 13 };
  const uint8_t  coordsX[NUM_PIXELS]             { 152, 224, 252, 210, 211, 184, 169, 161, 89, 121, 138, 102, 61, 19, 13, 57, 82, 29, 0, 36, 63, 111, 79, 83, 158, 129, 118, 160, 196, 255, 212, 163, 203, 250 };
  const uint8_t  coordsY[NUM_PIXELS]             { 120, 101, 69, 7, 48, 90, 50, 7, 0, 27, 83, 62, 37, 35, 84, 78, 112, 125, 154, 185, 149, 140, 195, 236, 255, 226, 179, 198, 223, 181, 183, 156, 144, 135 };
  const uint8_t  angles[NUM_PIXELS]              { 255, 246, 237, 214, 223, 232, 208, 199, 176, 185, 193, 170, 161, 152, 138, 147, 132, 123, 114, 100, 108, 94, 85, 76, 53, 62, 70, 47, 38, 15, 23, 32, 9, 0 };
#elif defined(PRODUCT_KRAKEN64)
  const uint8_t  coordsX[NUM_PIXELS]             { 151, 188, 199, 199, 171, 147, 131, 119, 124, 179, 200, 217, 237, 249, 242, 234, 255, 148, 175, 177, 150, 143, 171, 153, 155, 106, 110, 102, 75, 86, 106, 108, 88, 90, 84, 78, 107, 98, 121, 128, 80, 69, 134, 159, 192, 202, 195, 218, 61, 32, 18, 26, 39, 20, 3, 0, 48, 52, 61, 54, 33, 20, 7, 8 };
  const uint8_t  coordsY[NUM_PIXELS]             { 190, 204, 225, 252, 255, 236, 216, 191, 166, 147, 154, 170, 173, 156, 131, 107, 106, 148, 121, 86, 81, 63, 56, 36, 17, 145, 120, 96, 55, 23, 18, 0, 2, 112, 77, 31, 35, 49, 51, 30, 124, 103, 101, 98, 80, 58, 40, 69, 134, 124, 107, 81, 56, 43, 50, 70, 156, 180, 202, 219, 213, 195, 197, 215 };
  const uint8_t  angles[NUM_PIXELS]              { 0, 249, 241, 232, 223, 200, 208, 217, 226, 235, 212, 203, 194, 185, 176, 162, 171, 180, 188, 197, 174, 165, 156, 147, 139, 124, 133, 142, 151, 136, 128, 119, 110, 101, 78, 86, 95, 104, 113, 99, 90, 81, 72, 63, 40, 49, 58, 67, 75, 52, 43, 34, 25, 17, 2, 11, 20, 29, 38, 14, 6, 255, 246, 237 };
  const uint8_t  body[NUM_PIXELS]                { 0, 16, 32, 48, 64, 80, 96, 112, 128, 143, 159, 175, 191, 207, 223, 239, 255, 143, 159, 175, 191, 207, 223, 239, 255, 143, 159, 175, 191, 207, 223, 239, 255, 143, 159, 175, 191, 207, 223, 239, 143, 159, 175, 191, 207, 223, 239, 255, 143, 159, 175, 191, 207, 223, 239, 255, 143, 159, 175, 191, 207, 223, 239, 255 };
  static_assert(NUM_PIXELS == ARRAY_SIZE2(body), "");
  // For reference purposes...
  // const uint8_t head[9]      {  0,  1,  2,  3,  4,  5,  6,  7, 8 };
  // const uint8_t tentacle0[8] {  9, 10, 11, 12, 13, 14, 15, 16    };
  // const uint8_t tentacle1[8] { 17, 18, 19, 20, 21, 22, 23, 24    };
  // const uint8_t tentacle2[8] { 25, 26, 27, 28, 29, 30, 31, 32    };
  // const uint8_t tentacle3[8] { 33, 34, 35, 36, 37, 38, 39        };
  // const uint8_t tentacle4[8] { 40, 41, 42, 43, 44, 45, 46, 47    };
  // const uint8_t tentacle5[8] { 48, 49, 50, 51, 52, 53, 54, 55    };
  // const uint8_t tentacle6[8] { 56, 57, 58, 59, 60, 61, 62, 63    };
#else
  #error "Unknown / Unsupported fibonacci product ... no mappings defined"
#endif

#if IS_FIBONACCI
  static_assert(NUM_PIXELS == ARRAY_SIZE2(physicalToFibonacci), "");
  static_assert(NUM_PIXELS == ARRAY_SIZE2(fibonacciToPhysical), "");
#endif
#if HAS_COORDINATE_MAP
  static_assert(NUM_PIXELS == ARRAY_SIZE2(coordsX), "");
  static_assert(NUM_PIXELS == ARRAY_SIZE2(coordsY), "");
  static_assert(NUM_PIXELS == ARRAY_SIZE2(angles), "");
#endif

// What is HAS_RADIUS_PROXY?  It could be the actual radius, or it could be something
// that is used "in place of" the radius during effects processing.
// Use reference to original array; avoids repeating preprocessor checks throughout the code....
#if defined(PRODUCT_KRAKEN64)
  #define HAS_RADIUS_PROXY 1
  static const auto (&radiusProxy)[NUM_PIXELS] = body;
  static const uint8_t RADII_SCALE_DIVISOR    { 1 }; // body[] values are already in range [0..255]
  static const uint8_t RADII_SCALE_MULTIPLIER { 1 }; // body[] values are already in range [0..255]
#elif IS_FIBONACCI
  #define HAS_RADIUS_PROXY 1

  static const auto (&radiusProxy)[NUM_PIXELS] = physicalToFibonacci;

  static_assert(
    ((NUM_PIXELS & (NUM_PIXELS-1)) == 0) ||
    (NUM_PIXELS == 34),
    "RADII_SCALE_MULTIPLIER / RADII_SCALE_DIVISOR need updates to handle other non-power-of-two NUM_PIXELS"
    );
  static const uint8_t RADII_SCALE_DIVISOR    { (NUM_PIXELS <= 256u) ? 1 : (NUM_PIXELS / 256u) };
  static const uint8_t RADII_SCALE_MULTIPLIER {
    (NUM_PIXELS >= 256) ? 1 :
    (NUM_PIXELS ==  34) ? 8 : // Careful .. use saturating multiplier or manually limit to 256...
    ((NUM_PIXELS & (NUM_PIXELS-1)) == 0) ? (256u/NUM_PIXELS) :
    0
  };
  static_assert(RADII_SCALE_MULTIPLIER != 0, "");
#else
  #define HAS_RADIUS_PROXY 0 // default is that this is not supported....
#endif

#if HAS_RADIUS_PROXY
  static_assert(HAS_COORDINATE_MAP, "HAS_RADIUS_PROXY also requires coordinate map");
#endif

#if IS_FIBONACCI // drawSpiralLine() uses angles[] and physicalToFibonacci[]
void drawSpiralLine(uint8_t angle, int step, CRGB color)
{
  int startIndex = 0;
  int smallestAngleDifference = 255;

  // find the outermost led closest to the desired angle
  for (int i = 0; i < NUM_PIXELS; i++) {
    int j = physicalToFibonacci[i];
    if (j < step) continue;
    if (!(j + step >= NUM_PIXELS)) continue; // not outermost
    uint8_t a = angles[i];
    if (a == angle) startIndex = i;
    else if (angle - a > 0 && angle - a < smallestAngleDifference) {
      smallestAngleDifference = angle - a;
      startIndex = i;
    }
  }

  // draw the starting LED
  leds[startIndex] += color;

  // draw to center from outer start
  int f = physicalToFibonacci[startIndex];
  while (f - step >= 0 && f - step < NUM_PIXELS) {
    leds[fibonacciToPhysical[f]] += color;
    f = f - step;
  }
}
#endif

#if HAS_RADIUS_PROXY // setPixelAR() uses radiusProxy[]
// given an angle and radius (and delta for both), set pixels that fall inside that range
void setPixelAR(uint8_t angle, uint8_t dAngle, uint8_t radius, uint8_t dRadius, CRGB color)
{
  uint8_t endRadius   = qadd8(radius, dRadius);
  uint8_t startRadius = qsub8(radius, dRadius);

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
  // TODO: Change from pre-processor defines to `static const bool` values where possible
    uint8_t ro = radiusProxy[i];
    // only mess with the pixel when it's radius is within the target radius
    if (ro <= endRadius && ro >= startRadius) {
      // Get pixel's angle (unit256)
      uint8_t ao = angles[i];
      // set adiff to abs(ao - angle) ... relies on unsigned underflow resulting in larger value
      uint8_t adiff = min(sub8(ao,angle), sub8(angle, ao));
      // only mess with the pixel when it's angle is within range of target
      if (adiff <= dAngle) {
        leds[i] = color;
      }
    }
  }
}
#endif

#if HAS_RADIUS_PROXY // andPixelAR() uses radiusProxy[]
// given an angle and radius (and delta for both), add color to pixels that fall inside that range
void andPixelAR(uint8_t angle, uint8_t dAngle, uint8_t startRadius, uint8_t endRadius, CRGB color)
{
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint8_t ro = radiusProxy[i];
    // only mess with the pixel when it's radius is within the target radius
    if (ro <= endRadius && ro >= startRadius) {
      // Get pixel's angle (unit256)
      uint8_t ao = angles[i];
      // set adiff to abs(ao - angle) ... relies on unsigned underflow resulting in larger value
      uint8_t adiff = min(sub8(ao,angle), sub8(angle, ao));
      // only mess with the pixel when it's angle is within range of target
      if (adiff <= dAngle) {
        leds[i] += color;
      }
    }
  }
}
#endif

#if HAS_RADIUS_PROXY // antialiasPixelAR() uses angles[] and radiusProxy[]
// given an angle and radius (and delta for both), set pixels that fall inside that range,
// fading the color from full-color at center, to off (black) at the outer edges.
void antialiasPixelAR(uint8_t angle, uint8_t dAngle, uint8_t startRadius, uint8_t endRadius, CRGB color, CRGB leds[] = leds, int _NUM_PIXELS = NUM_PIXELS)
{
  // NOTE:
  // An earlier version of this routine had significant bugs.
  // Do NOT use the version which does qsub8(max(...), min()),
  // as it does not handle angle overflow well.
  // This version does it properly:
  // 1. subtract both ways
  // 2. note that unsigned underlow will make the negative result really large instead
  // 3. take smaller value
  // This is the absolute offset from the target angle
  for (uint16_t i = 0; i < _NUM_PIXELS; i++) {
    uint8_t ro = radiusProxy[i];
    // only mess with the pixel when it's radius is within the target radius
    if (ro <= endRadius && ro >= startRadius) {
      // Get pixel's angle (unit256)
      uint8_t ao = angles[i];
      // set adiff to abs(ao - angle) ... relies on unsigned underflow resulting in larger value
      uint8_t adiff = min(sub8(ao,angle), sub8(angle, ao));
      // only mess with the pixel when it's angle is within range of target
      if (adiff <= dAngle) {
        // map the intensity of the color so it fades to black at edge of allowed angle
        uint8_t fade = map(adiff, 0, dAngle, 0, 255);
        CRGB faded = color;
        // fade the target color based on how far the angle was from the target
        faded.fadeToBlackBy(fade);
        // add the faded color (as an overlay) to existing colors
        leds[i] += faded;
      }
    }
  }
}
#endif

// TODO - anglePalette() from Fib32 sets `hues = 256 / NUM_PIXELS' ... which is ZERO(!)
//        The other similar functions are hard-coded to `hues = 1` even on Fib32.
//        Likely a bug in Fib32 branch for this one function?
//        Check if other branches did similar, or set to hard-coded value of 1?
#if HAS_COORDINATE_MAP // anglePalette() uses angles[]
void anglePalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint16_t x = angles[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (x * hues));
  }
}
#endif

// TODO - consider adding pre-computed radii[], enabled anytime HAS_COORDINATE_MAP is true
#if HAS_RADIUS_PROXY // radiusPalette() uses radiusProxy[]
void radiusPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint8_t r = ((unsigned)(radiusProxy[i] * RADII_SCALE_MULTIPLIER)) / RADII_SCALE_DIVISOR;
    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (r * hues));
  }
}
#endif

#if HAS_COORDINATE_MAP // xPalette() uses coordsX[]
void xPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint16_t x = coordsX[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (x * hues));
  }
}
#endif

#if HAS_COORDINATE_MAP // yPalette() uses coordsY[]
void yPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint16_t y = coordsY[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - (y * hues));
  }
}
#endif

#if HAS_COORDINATE_MAP // xyPalette() uses coordsX[] and coordsY[]
void xyPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint16_t x = coordsX[i];
    uint16_t y = coordsY[i];

    leds[i] = ColorFromPalette(palettes[currentPaletteIndex], beat8(speed) - ((x + y) * hues));
  }
}
#endif

#if HAS_COORDINATE_MAP // angleGradientPalette() uses angles[]
void angleGradientPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint16_t x = angles[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (x * hues));
  }
}
#endif

#if HAS_RADIUS_PROXY // radiusGradientPalette() uses radiusProxy[]
void radiusGradientPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint8_t r = ((unsigned)(radiusProxy[i] * RADII_SCALE_MULTIPLIER)) / RADII_SCALE_DIVISOR;
    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (r * hues));
  }
}
#endif

#if HAS_COORDINATE_MAP // xGradientPalette() uses coordsX[]
void xGradientPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint16_t x = coordsX[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (x * hues));
  }
}
#endif

#if HAS_COORDINATE_MAP // yGradientPalette() uses coordsY[]
void yGradientPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint16_t y = coordsY[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - (y * hues));
  }
}
#endif

#if HAS_COORDINATE_MAP // xyGradientPalette() uses coordsX[] and coordsY[]
void xyGradientPalette() {
  uint16_t hues = 1;

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint16_t x = coordsX[i];
    uint16_t y = coordsY[i];

    leds[i] = ColorFromPalette(gCurrentPalette, beat8(speed) - ((x + y) * hues));
  }
}
#endif

#if IS_FIBONACCI // drawAnalogClock() calls antialiasPixelAR(), which requires physicalToFibonacci[]
void drawAnalogClock() {

  static_assert(NUM_PIXELS >= 32, "Update to drawAnalogClock() required to support fewer pixels");
  const uint8_t hourRadius   = NUM_PIXELS / 8 * 3; //  96 designed for 256 pixels ==> 3/8
  const uint8_t minuteRadius = NUM_PIXELS / 4 * 3; // 192 designed for 256 pixels ==> 3/4
  const uint8_t secondRadius = NUM_PIXELS - 1;     // 255 designed for 256 pixels ==> all pixels

  const uint8_t hourHandWidth   = 8; // angle @ unit256 ~= 11.25000 degrees
  const uint8_t minuteHandWidth = 7; // angle @ unit256 ~=  9.84375 degrees
  const uint8_t secondHandWidth = 6; // angle @ unit256 ~=  8.43750 degrees

  const float degreesPerSecond = 256.0 / 60.0;
  const float degreesPerMinute = 256.0 / 60.0;
  const float degreesPerHour   = 256.0 / 12.0;

  static uint8_t hourAngle   = 0;
  static uint8_t minuteAngle = 0;
  static uint8_t secondAngle = 0;

  EVERY_N_MILLIS(100) {
    float second = timeClient.getSeconds();
    float minute = timeClient.getMinutes() + (second / 60.0);
    float hour   = timeClient.getHours()   + (minute / 60.0);

    hourAngle   = 256u - hour   * degreesPerHour;
    minuteAngle = 256u - minute * degreesPerMinute;
    secondAngle = 256u - second * degreesPerSecond;
  }

  fadeToBlackBy(leds, NUM_PIXELS, clockBackgroundFade);

  antialiasPixelAR(secondAngle, secondHandWidth, 0, secondRadius, CRGB::Blue );
  antialiasPixelAR(minuteAngle, minuteHandWidth, 0, minuteRadius, CRGB::Green);
  antialiasPixelAR(hourAngle,   hourHandWidth,   0, hourRadius,   CRGB::Red  );
  leds[0] = CRGB::Red;
}
#endif

// TODO: `drawSpiralAnalogClock13_21_and_34()` -- config to disable the seconds hand?

#if IS_FIBONACCI // drawSpiralAnalogClock*() calls drawSpiralLine(), which requires fibonacci
void drawSpiralAnalogClock(uint8_t step_h, uint8_t step_m, uint8_t step_s) {
  static uint8_t hourAngle   = 0;
  static uint8_t minuteAngle = 0;
  static uint8_t secondAngle = 0;

  const float degreesPerSecond = 256.0 / 60.0;
  const float degreesPerMinute = 256.0 / 60.0;
  const float degreesPerHour   = 256.0 / 12.0;

  EVERY_N_MILLIS(100) {
    float second = timeClient.getSeconds();
    float minute = timeClient.getMinutes() + (second / 60.0);
    float hour   = timeClient.getHours()   + (minute / 60.0);

    hourAngle   = 256u - hour   * degreesPerHour;
    minuteAngle = 256u - minute * degreesPerMinute;
    secondAngle = 256u - second * degreesPerSecond;
  }

  drawSpiralLine(secondAngle, step_s, CRGB(0, 0, 2));
  drawSpiralLine(minuteAngle, step_m, CRGB(0, 2, 0));
  drawSpiralLine(hourAngle,   step_h, CRGB(2, 0, 0));
}
void drawSpiralAnalogClock(uint8_t step) {
  drawSpiralAnalogClock(step, step, step);
}
void drawSpiralAnalogClock13() {
  fadeToBlackBy(leds, NUM_PIXELS, clockBackgroundFade);
  drawSpiralAnalogClock(13, 13, 13);
}
void drawSpiralAnalogClock21() {
  fadeToBlackBy(leds, NUM_PIXELS, clockBackgroundFade);
  drawSpiralAnalogClock(21, 21, 21);
}
void drawSpiralAnalogClock34() {
  fadeToBlackBy(leds, NUM_PIXELS, clockBackgroundFade);
  drawSpiralAnalogClock(34, 34, 34);
}
void drawSpiralAnalogClock55() {
  fadeToBlackBy(leds, NUM_PIXELS, clockBackgroundFade);
  drawSpiralAnalogClock(55, 55, 55);
}
void drawSpiralAnalogClock89() {
  fadeToBlackBy(leds, NUM_PIXELS, clockBackgroundFade);
  drawSpiralAnalogClock(89, 89, 89);
}
void drawSpiralAnalogClock21and34() {
  fadeToBlackBy(leds, NUM_PIXELS, clockBackgroundFade);
  drawSpiralAnalogClock(21, 21, 21);
  drawSpiralAnalogClock(34, 34, 34);
}
void drawSpiralAnalogClock13_21_and_34() {
  fadeToBlackBy(leds, NUM_PIXELS, clockBackgroundFade);
  drawSpiralAnalogClock(34, 21, 13);
}
void drawSpiralAnalogClock34_21_and_13() {
  fadeToBlackBy(leds, NUM_PIXELS, clockBackgroundFade);
  drawSpiralAnalogClock(13, 21, 34);
}
#endif


#endif // IS_FIBONACCI || HAS_COORDINATE_MAP
