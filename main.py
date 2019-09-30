# -*- coding: utf-8 -*-
#!/usr/bin/python3


#import psutil
#from psutil._common import bytes2human
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

#ID-CHAVE DE ACESSO:  AKIA23F57SOWPXV3WMFH

#PRIVATE-KEY :  BzOH6cFYKFLw1ktdoAbqDiKJoq1JRmk9C3eb7r2x

#ARN topico criado: arn:aws:sns:us-east-1:745575257004:LDR_Topic

#endpoint : a1h982mdzwu26k-ats.iot.us-east-1.amazonaws.com

myMQTTClient = AWSIoTMQTTClient("arn:aws:iot:us-east-1:745575257004:thing/LDR_IoT", useWebsocket = True)

myMQTTClient.configureEndpoint("a1h982mdzwu26k-ats.iot.us-east-1.amazonaws.com", 443)

myMQTTClient.configureCredentials("root-CA.crt")

myMQTTClient.configureOfflinePublishQueueing(-1)

myMQTTClient.configureDrainingFrequency(2)

myMQTTClient.configureConnectDisconnectTimeout(10)

myMQTTClient.configureMQTTOperationTimeout(5)

myMQTTClient.connect()

def main():

	#Infos
	#CPU_system 			= psutil.cpu_count()
	#CPU_physical_cores	= psutil.cpu_count(logical=False)

	#Controle = input("Qual o consumo desejado?")
	Email = True
	presenca = 1
	trigger = 1
	while(True):
		'''
		#%
		print("\nCPU	#######\n")
		#CPU_percent = psutil.cpu_percent(interval=2)
		print('CPU_percent 	: %.2f' % CPU_percent)

		#(MHz)
		CPU_freq	= psutil.cpu_freq(percpu = False)
		print('CPU_freq Cur 	: %.2f' % CPU_freq[0])
		print('CPU_freq Min 	: %.2f' % CPU_freq[1])
		print('CPU_freq Max 	: %.2f' % CPU_freq[2])

		#Memory
		Memory 		= psutil.virtual_memory()
		print("\nMemory	#######\n")
		print('Total 		: %s' % bytes2human(Memory[0]))
		print('Available	: %s' % bytes2human(Memory[1]))
		print('Percent 	: %s' % Memory[2])
		print('Used 		: %s' % bytes2human(Memory[3]))
		print('Free 		: %s' % bytes2human(Memory[4]))

		#Disk
		Disk		= psutil.disk_usage('/')
		print("\nDisk	#######\n")
		print('Total 		: %s' % bytes2human(Disk[0]))
		print('Used		: %s' % bytes2human(Disk[1]))
		print('Free 		: %s' % bytes2human(Disk[2]))
		print('Percent 	: %s' % Disk[3])

		#Batery
		Battery 	= psutil.sensors_battery()
		print("\nBattery #######\n")
		print('Percent 	: %s' % Battery[0])
		print('Secsleft	: %s' % Battery[1])
		print('Power_plugged 	: %s' % Battery[2])
		'''
		if(presenca >= trigger) and (Email == True):
			
			myMQTTClient.publish("$aws/things/LDR_IoT/shadow/update", "{'Pessoa no ambiente'}", 0)
			
			print("\nAlerta enviado ######\n")
			
			Email = False

			myMQTTClient.disconnect()

			break

if __name__=="__main__":
	main()
