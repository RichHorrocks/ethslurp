#include <stdio.h>
#include "rpc.h"

//-----------------------------------------------------
IPCSocket::IPCSocket(SFString const& _path): m_path(_path)
{
	if (_path.GetLength() >= sizeof(sockaddr_un::sun_path))
		{ outErr << "Error opening IPC: socket path is too long!" << "\n"; exit(0); }

	struct sockaddr_un saun;
	memset(&saun, 0, sizeof(sockaddr_un));
	saun.sun_family = AF_UNIX;
	strcpy(saun.sun_path, (const char*)_path);
	saun.sun_len = sizeof(struct sockaddr_un);

	if ((m_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		{ outErr << "Error creating IPC socket object" << "\n"; exit(0); }

	if (connect(m_socket, reinterpret_cast<struct sockaddr const*>(&saun), sizeof(struct sockaddr_un)) < 0)
		{ outErr << "Error connecting to IPC socket: " << "\n"; exit(0); }

	m_fp = fdopen(m_socket, "r");
}

//-----------------------------------------------------
SFString IPCSocket::sendRequest(SFString const& _req)
{
	send(m_socket, (const char*)_req, _req.GetLength(), 0);

	char c;
	SFString response;
	while ((c = fgetc(m_fp)) != EOF)
	{
		if (c != '\n')
			response += c;
		else
			break;
	}
	return response;
}

//-----------------------------------------------------
CReceipt RPCSession::eth_getTransactionReceipt(SFString const& _transactionHash)
{
	CReceipt receipt;
#if 0
	SFString const result = rpcCall("eth_getTransactionReceipt", { _transactionHash });
	if (result.IsEmpty())
	{
		outErr << "Result from eth_getTransactionReceipt call is empty. Quitting...\n";
		exit(0);
	}
	receipt.gasUsed = result["gasUsed"];
	receipt.contractAddress = result["contractAddress"];
	for (auto const& log: result["logs"])
	{
		LogEntry entry;
		entry.address = log["address"];
		entry.data = log["data"];
		for (auto const& topic: log["topics"])
			entry.topics.push_back(topic);
		receipt.logEntries.push_back(entry);
	}
#endif
	return receipt;
}

//-----------------------------------------------------
SFString RPCSession::rpcCall(SFString const& _methodName, SFString const& _args)
{
	SFString request = "{\"jsonrpc\":\"2.0\",\"method\":\"" + _methodName + "\",\"params\":[";
	SFString args = _args;
	while (!args.IsEmpty())
	{
		request += "\"" + nextTokenClear(args, '|') + "\"";
		if (!args.IsEmpty())
			request += ",";
	}
	request += "],\"id\":" + asString(m_rpcSequence) + "}";
	++m_rpcSequence;

	SFString result = m_ipcSocket.sendRequest(request);
	if (result.Contains("error"))
	{
		outErr << "Error on JSON-RPC call: " << result << "\n";
		exit(0);
	}
	return result;
}

//-----------------------------------------------------
SFString RPCSession::eth_getCode(SFString const& _address, SFString const& _blockNumber)
{
	return rpcCall("eth_getCode", _address + "|" + _blockNumber);
}

//-----------------------------------------------------
SFString RPCSession::eth_getBalance(SFString const& _address, SFString const& _blockNumber)
{
	return rpcCall("eth_getBalance", _address + "|" + _blockNumber);
}

//-----------------------------------------------------
SFString RPCSession::eth_getStorageRoot(SFString const& _address, SFString const& _blockNumber)
{
	return rpcCall("eth_getStorageRoot", _address + "|" + _blockNumber);
}

//-----------------------------------------------------
RPCSession::RPCSession(const SFString& _path) : m_ipcSocket(_path)
{
}

