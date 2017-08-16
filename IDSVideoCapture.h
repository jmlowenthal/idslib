#include <opencv2/videoio.hpp>
#include <ueye.h>

using namespace cv;

#ifndef IDSVIDEOCAPTURE_H
#define IDSVIDEOCAPTURE_H

#define NUM_BUFFERS 2

class IDSVideoCapture : public VideoCapture {

private:

	HIDS m_CamId = 0;

	CAMINFO m_CameraInfo;

	SENSORINFO m_SensorInfo;

	UINT m_ImageFormatListLength;

	IMAGE_FORMAT_LIST *m_ImageFormatList;
	
	int m_Width;
	
	int m_Height;

	long m_StartTime;

	char* m_LockedMemory = nullptr;
	
	char* m_ImageMemoryAddr[NUM_BUFFERS] = { nullptr };
	
	int m_ImageMemoryId[NUM_BUFFERS];

public:

	IDSVideoCapture();

	IDSVideoCapture(int index);

	virtual ~IDSVideoCapture();

	virtual bool open(int index);

	virtual void release();

	virtual bool isOpened() const;

	virtual bool grab();

	virtual bool retrieve(OutputArray image, int flag = 0);

	virtual double get(int propid) const;

	virtual bool set(int propId, double value);

};

#endif // #ifdef IDSVIDEOCAPTURE_H