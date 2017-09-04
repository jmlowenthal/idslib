#include <opencv2/videoio.hpp>
#include <ueye.h>

using namespace cv;

#ifndef IDSVIDEOCAPTURE_H
#define IDSVIDEOCAPTURE_H

#define NUM_BUFFERS 2

/*
	Extends OpenCV's VideoCapture to fetch camera frames from an IDS uEye
	camera.
*/
class IDSVideoCapture : public VideoCapture {

private:

	/*
		API handle for camera.
	*/
	HIDS m_CamId = 0;
	
	/*
		API basic camera information structure, including serial number and
		camera name.
	*/
	CAMINFO m_CameraInfo;

	/*
		API sensor information structure, including serial number, sensor name,
		maximum dimensions and flags to indicate which features are supported.
	*/
	SENSORINFO m_SensorInfo;
	
	/*
		Output frame width.
	*/
	int m_Width;
	
	/*
		Output frame height.
	*/
	int m_Height;

	/*
		Time stamp for camera recording start.
	*/
	long m_StartTime;

	/*
		Pointer to currently locked frame.
	*/
	char* m_LockedMemory = nullptr;
	
	/*
		Pointers to all buffers in memory that are given to uEye API.
	*/
	char* m_ImageMemoryAddr[NUM_BUFFERS] = { nullptr };
	
	/*
		API references to image memory.
	*/
	int m_ImageMemoryId[NUM_BUFFERS];

public:

	/*
		Initialise camera interface without opening the camera.
	*/
	IDSVideoCapture();

	/*
		Initialise camera interface and open camera with given index.
	*/
	IDSVideoCapture(int index);

	/*
		Destroy the camera interface after releasing its resources.
	*/
	virtual ~IDSVideoCapture();

	/*
		Open the camera with the given index.
	*/
	virtual bool open(int index);

	/*
		Release the interfaces resources and close the camera.
	*/
	virtual void release();

	/*
		Returns true if the camera has been successfully opened.
	*/
	virtual bool isOpened() const;

	/*
		Fetch a frame from the camera internally.
	*/
	virtual bool grab();

	/*
		Collect the last frame last fetched with `grab()` into the provided
		output image.
	*/
	virtual bool retrieve(OutputArray image, int flag = 0);

	/*
		Get camera property. Implemented values are:
		-	`CAP_PROP_POS_MSEC`
		-	`CAP_PROP_POS_FRAMES`
		-	`CAP_PROP_POS_AVI_RATIO`
		-	`CAP_PROP_FRAME_WIDTH`
		-	`CAP_PROP_FRAME_HEIGHT`
		-	`CAP_PROP_FPS`
		-	`CAP_PROP_FOURCC`
		-	`CAP_PROP_FORMAT`
	*/
	virtual double get(int propid) const;

	/*
		WARNING: not implemented
	*/
	virtual bool set(int propId, double value);

};

#endif // #ifdef IDSVIDEOCAPTURE_H