using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Windows;

namespace QuadcopterGui
{
  /// <summary>
  ///   Class for the network communication. This class is able to connect to the server and send the values.
  /// </summary>
  internal class NetworkConnection
  {
    private readonly int _port;
    private readonly IPAddress _serverIpAddress;
    private TcpClient _client;
    private NetworkStream _stream;
    private bool _isConnected;
    private StreamWriter _streamWriter;

    /// <summary>
    ///   Constructor to initialise this class
    /// </summary>
    /// <param name="serverIpAddress">Serveraddress</param>
    /// <param name="port">TCP Port of the server</param>
    public NetworkConnection(IPAddress serverIpAddress, int port)
    {
      _serverIpAddress = serverIpAddress;
      _port = port;
    }

    /// <summary>
    ///   Connect to the server
    /// </summary>
    /// <returns>True if successfully connected</returns>
    public bool Connect()
    {
      _client = new TcpClient();
      if (_serverIpAddress != null)
      {
        try
        {
          _client.Connect(_serverIpAddress, _port);
          _stream = _client.GetStream();
          _streamWriter = new StreamWriter(_stream);
        }
        catch (Exception exception)
        {
          return false;
        }
        _isConnected = true;
        return true;
      }
      return false;
    }

    /// <summary>
    ///   Disconnect from server
    /// </summary>
    public void Disconnect()
    {
      _streamWriter.Close();
      _client.Close();
      _stream = null;
      _isConnected = false;
    }

    /// <summary>
    ///   Sends a string to the server
    /// </summary>
    /// <param name="data">JSON string to send</param>
    public void Send(string data)
    {
      if (_isConnected) // only send if we are connected
      {
        if (_streamWriter != null)
        {
          _streamWriter.Write(data);
        }
      }
    }
  }
}
