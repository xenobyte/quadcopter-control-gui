using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace QuadcopterGui
{
  /// <summary>
  /// Interaktionslogik für MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    private NetworkConnection _networkConnection = null;

    public MainWindow()
    {
      InitializeComponent();
    }

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

    private void SendButton_OnClick(object sender, RoutedEventArgs e)
    {
      const string json = "{ \"Angle\" : { " +
                          "\"P\": 23.23,  \"I\": 23.23, \"D\": 23.23, } " +
                          "}";
      _networkConnection.Send(json);
    }
  }
}
