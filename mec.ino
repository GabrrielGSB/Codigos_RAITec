#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

bool calibration = false;

int Timer1 = 0,
    Timer2 = 0,
    count  = 0;

float KalmanGain, KalmanAngleRoll, KalmanUncertaintyAngleRoll, KalmanAnglePitch, KalmanUncertaintyAnglePitch,
      RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw, RateRoll, RatePitch, RateYaw,
      AceX, AceY, AceZ, Temp,
      GyrXc, GyrYc, GyrZc,
      AngleRoll, AnglePitch;

Adafruit_MPU6050 mpu;

void setup() 
{
  Serial.begin(115200);
  MPUconfigSetup() ;
  CalibrarMPU();

  KalmanGain = 0;
	KalmanAngleRoll  = 0, KalmanUncertaintyAngleRoll  = 2*2;
	KalmanAnglePitch = 0, KalmanUncertaintyAnglePitch = 2*2;
}

void loop() 
{
  MPUgetSignalsLoop();
  Kalman1D(KalmanAngleRoll, KalmanUncertaintyAngleRoll, RateRoll, AngleRoll);   
  Kalman1D(KalmanAnglePitch, KalmanUncertaintyAnglePitch, RatePitch, AnglePitch);
  DisplaySerialMpuData();
}

void MPUconfigSetup() 
{
	if (!mpu.begin()) 
	{	
		while (1) 
		{
			Serial.println("error");
	  	yield();
		}
	}
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);//2G, 4G, 8G, 16G
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);     //250deg/s, 500deg/s, 1000deg/s, 2000deg/s
	mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);  //5Hz, 10Hz, 21Hz, 44Hz, 94Hz, 184Hz, 260Hz
}

void CalibrarMPU()
{
 	for (int Passo = 0 ; Passo < 2000; Passo++) 
	{
		Serial.printf("Calibrando: %d\n", Passo);

		MPUgetSignalsLoop();
		RateCalibrationRoll  += RateRoll;
		RateCalibrationPitch += RatePitch;
		RateCalibrationYaw   += RateYaw;
		delay(1);
	}
	calibration = true;
}

void MPUgetSignalsLoop() 
{	
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  AceX = a.acceleration.x / 9.81; // m/s^2...
  AceY = a.acceleration.y / 9.81; // ...
  AceZ = a.acceleration.z / 9.81; // ...
 
  RateRoll  = g.gyro.x * 57.3; // °/s...
  RatePitch = g.gyro.y * 57.3; // ...
  RateYaw   = g.gyro.z * 57.3; // ...
  
  Temp = temp.temperature; // °C
  
  AngleRoll  =  atan(AceY/sqrt(AceX*AceX + AceZ*AceZ))*1/(3.142/180);
  AnglePitch = -atan(AceX/sqrt(AceY*AceY + AceZ*AceZ))*1/(3.142/180);
  
  if (calibration = true)
  {
	RateRoll  -= (RateCalibrationRoll/2000)  - 1.2;
	RatePitch -= (RateCalibrationPitch/2000) - 1.5;
	RateYaw   -= (RateCalibrationYaw/2000)   - 0;
	AceX -= 0.02;
	AceY -= 0.03;
	AceZ -= 0.13;
  }
}

void DisplaySerialMpuData()
{	
	if ((millis() - Timer1) >= 10)
	{
		Timer1 = millis();
		
    Serial.printf("AnguloRoll: %f, AnguloPitch: %f,KalmanRoll: %f, KalmanPitch: %f, Tempo: %d, \n", 
                  AngleRoll, AnglePitch, KalmanAngleRoll, KalmanAnglePitch, Timer1);
	}
}

void DisplayPlotterMpuData()
{
	Kalman1D(KalmanAngleRoll, KalmanUncertaintyAngleRoll, RateRoll, AngleRoll);     // chamada da função do filtro afim...
  Kalman1D(KalmanAnglePitch, KalmanUncertaintyAnglePitch, RatePitch, AnglePitch); // ...

	if ((millis() - Timer2) >= 10)
  {
		Timer2 = millis();
		Serial.printf("Angle_Roll: %f, Angle_Pitch: %f, Kalman_Angle_Roll: %f, Kalman_Angle_Pitch: %f\n", 
		AngleRoll, AnglePitch, KalmanAngleRoll, KalmanAnglePitch);

		//Serial.println(GyrZ);
	}
}

void Kalman1D(float &KalmanState,float &KalmanUncertainty, const float &KalmanInput, 
					    const float &KalmanMeasurement)
{
	KalmanState       = KalmanState + 0.004*KalmanInput;
	KalmanUncertainty = KalmanUncertainty + 0.004*0.004*4*4;
	KalmanGain        = KalmanUncertainty*1/(1*KalmanUncertainty + 3*3);
	KalmanState       = KalmanState + KalmanGain*(KalmanMeasurement - KalmanState);
	KalmanUncertainty = (1 - KalmanGain)*KalmanUncertainty;
}

