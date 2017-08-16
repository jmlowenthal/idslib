#include "IDSVideoCapture.h"
#include <opencv2/core.hpp>

IDSVideoCapture::IDSVideoCapture() {
	
}

IDSVideoCapture::IDSVideoCapture(int index) {
	open(index);
}

IDSVideoCapture::~IDSVideoCapture() {
	release();
}

bool IDSVideoCapture::open(int _index) {

	HIDS index = _index;

	release();

	if (is_InitCamera(&index, NULL) == IS_SUCCESS) {
		
		m_CamId = index;
		is_GetCameraInfo(m_CamId, &m_CameraInfo);
		is_GetSensorInfo(m_CamId, &m_SensorInfo);

		for (int i = 0; i < NUM_BUFFERS; ++i) {
			is_AllocImageMem(
				m_CamId,
				m_SensorInfo.nMaxWidth,
				m_SensorInfo.nMaxHeight,
				8 * 3,
				&m_ImageMemoryAddr[i],
				&m_ImageMemoryId[i]
			);
			is_AddToSequence(
				m_CamId,
				m_ImageMemoryAddr[i],
				m_ImageMemoryId[i]
			);
		}

		m_Width = m_SensorInfo.nMaxWidth;
		m_Height = m_SensorInfo.nMaxHeight;

		is_SetDisplayMode(m_CamId, IS_SET_DM_DIB);
		is_SetColorMode(m_CamId, IS_CM_BGR8_PACKED);

		m_StartTime = time(NULL);
		is_CaptureVideo(m_CamId, IS_DONT_WAIT);

	}

	return isOpened();

}

void IDSVideoCapture::release() {
	if (isOpened()) {
		
		is_StopLiveVideo(m_CamId, IS_FORCE_VIDEO_STOP);
		
		is_ClearSequence(m_CamId);
		for (int i = 0; i < NUM_BUFFERS; ++i) {
			if (m_ImageMemoryAddr[i] != nullptr) {
				is_FreeImageMem(m_CamId, m_ImageMemoryAddr[i], m_ImageMemoryId[i]);
			}
		}
		
		is_ExitCamera(m_CamId);
		m_CamId = 0;
	}
}

bool IDSVideoCapture::isOpened() const {
	return m_CamId > 0;
}

bool IDSVideoCapture::grab() {
	if (isOpened()) {

		if (m_LockedMemory != nullptr) {
			is_UnlockSeqBuf(m_CamId, IS_IGNORE_PARAMETER, m_LockedMemory);
		}

		is_GetActSeqBuf(m_CamId, NULL, NULL, &m_LockedMemory);
		is_LockSeqBuf(m_CamId, IS_IGNORE_PARAMETER, m_LockedMemory);

		return true;

	}
	else {
		return false;
	}
}

bool IDSVideoCapture::retrieve(OutputArray image, int flag) {
	if (isOpened()) {
		Mat ref(m_Height, m_Width, CV_8UC3, (void*)m_LockedMemory);
		ref.copyTo(image);

		return true;
	}
	else {
		return false;
	}
}

double IDSVideoCapture::get(int propid) const {
	switch (propid) {

		case CAP_PROP_POS_MSEC: {
			return (double)(time(NULL) - m_StartTime) * 1e2;
		}

		case CAP_PROP_POS_FRAMES: {
			long count;
			is_GetVsyncCount(m_CamId, &count, NULL);
			return (double)count;
		}

		case CAP_PROP_POS_AVI_RATIO:
			return 0.0;

		case CAP_PROP_FRAME_WIDTH:
			return m_Width;

		case CAP_PROP_FRAME_HEIGHT:
			return m_Height;

		case CAP_PROP_FPS: {
			double fps;
			is_GetFramesPerSecond(m_CamId, &fps);
			return fps;
		}

		case CAP_PROP_FOURCC:
			return CAP_FFMPEG;

		case CAP_PROP_FORMAT:
			return CV_8UC3;

	}

	return 0.0;
}

bool IDSVideoCapture::set(int propId, double value) {
	switch (propId) {

		/*case CAP_PROP_FRAME_WIDTH:
			setResolution((int)value, m_Height);
			return true;

		case CAP_PROP_FRAME_HEIGHT:
			setResolution(m_Width, (int)value);
			return true;*/

	}

	return false;
}