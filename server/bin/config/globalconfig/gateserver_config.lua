GlobalConfig.Server.Gt = {

	Net = {
	
		dwIoType = 1,                               -- eIoType_CompletionPort
		dwRecvBufSize = 64 * 1024,
		dwSendBufSize = 64 * 1024,
		dwConnectCount = 100,
		dwNetEventPoolSize = 128 * 100,             -- dwConnectCount * 100
		dwNetEventListCount = 8,
		dwNetRecvEventBufSize = 20 * 1024 * 1024,
	},

	szListenIp = "127.0.0.1",
	dwListenPort = 5501,

	szClientListenIp = "192.168.1.97",
	dwClientListenPort = 5502,
};