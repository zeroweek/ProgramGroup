GlobalConfig.Server.Gs = {

	Net = {
	
		dwRecvBufSize = 64 * 1024,
		dwSendBufSize = 64 * 1024,
		dwConnectCount = 128,
		dwNetEventPoolSize = 128 * 100,             -- dwConnectCount * 100
		dwNetEventListCount = 8,
		dwNetRecvEventBufSize = 20 * 1024 * 1024,
	},

	szListenIp = "127.0.0.1",
	dwListenPort = 7001,

	szClientListenIp = "127.0.0.1",
	dwClientListenPort = 7002,
};