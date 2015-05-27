using System;
using System.Net;
using System.Windows;

namespace QuadcopterGui
{
  /// <summary>
  /// Interaktionslogik für MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    private NetworkConnection _networkConnection;

    public MainWindow()
    {
      InitializeComponent();
    }

    /// <summary>
    /// This is method is called if the connect button is pressed
    /// </summary>
    private void ConnectButton_OnClick(object sender, RoutedEventArgs e)
    {
      IPAddress ip = IPAddress.Parse(IpTextBox.Text);
      int port = Convert.ToInt32(PortTextBox.Text);
      _networkConnection = new NetworkConnection(ip, port);
      if (_networkConnection.Connect())
      {
        SendButton.IsEnabled = true;
      }
    }

    /// <summary>
    ///  This method is called if the send button is pressed
    /// </summary>
    private void SendButton_OnClick(object sender, RoutedEventArgs e)
    {
      // get values from sliders
      double angleP = Double.Parse(SliderAngleP.Value.ToString());
      double angleI = Double.Parse(SliderAngleI.Value.ToString());
      double angleD = Double.Parse(SliderAngleD.Value.ToString());

      double angleFreqP = Double.Parse(SliderAngleFreqP.Value.ToString());
      double angleFreqI = Double.Parse(SliderAngleFreqI.Value.ToString());
      double angleFreqD = Double.Parse(SliderAngleFreqD.Value.ToString());

      // build json string and round slider values to 2 of fractional digits
      string json = "{ \"Angle\" : { " +
                    "\"P\": " + Math.Round(angleP, 2) + ", \"I\": " + Math.Round(angleI, 2) + ", \"D\": " + Math.Round(angleD, 2) + ", } " +
                    "\"AngleFreq\" : { " +
                    "\"P\": " + Math.Round(angleFreqP, 2) + ", \"I\": " + Math.Round(angleFreqI, 2) + ", \"D\": " + Math.Round(angleFreqD, 2) + ", } " +
                    "}";
      _networkConnection.Send(json); // send to server
    }
  }
}
