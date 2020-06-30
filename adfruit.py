# Import standard python modules.
# Writed by LeHoangTuan
import sys
import requests
import socket
from netifaces import interfaces, ifaddresses, AF_INET
# This example uses the MQTTClient instead of the REST client
from Adafruit_IO import MQTTClient

# Set to your Adafruit IO key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
ADAFRUIT_IO_KEY = '6d532d2bf73649178e38026aedc648bb'

# Set to your Adafruit IO username.
# (go to https://accounts.adafruit.com to find your username)
ADAFRUIT_IO_USERNAME = 'LeHoangTuan'

# Set to the ID of the feed to subscribe to for updates.
FEED_ID = 'Door'

# Define callback functions which will be called when certain events happen.
def connected(client):
    """Connected function will be called when the client is connected to
    Adafruit IO.This is a good place to subscribe to feed changes.  The client
    parameter passed to this function is the Adafruit IO MQTT client so you
    can make calls against it easily.
    """
    # Subscribe to changes on a feed named Counter.
    print('Subscribing to Feed {0}'.format(FEED_ID))
    client.subscribe(FEED_ID)
    print('Waiting for feed data...')

def disconnected(client):
    """Disconnected function will be called when the client disconnects."""
    sys.exit(1)

def message(client, feed_id, payload):
    """Message function will be called when a subscribed feed has a new value.
    The feed_id parameter identifies the feed, and the payload parameter has
    the new value.
    """
    print('Feed {0} received new value: {1}'.format(feed_id, payload))
    data = {'duLieu': ''}
    data['duLieu'] = payload
    print(data)
    y = requests.post('http://127.0.0.1:3000/activity/voice',data)
    print(y.text)

# Create an MQTT client instance.
client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Setup the callback functions defined above.
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message

# Connect to the Adafruit IO server.
client.connect()
## getting the hostname by socket.gethostname() method
##hostname = socket.gethostname()
## getting the IP address using socket.gethostbyname() method
# ip_address = socket.gethostbyname(hostname)
# print("IP : ",ip_address)
## printing the hostname and ip_address
# for ifaceName in interfaces():
#     addresses = [i['addr'] for i in ifaddresses(ifaceName).setdefault(AF_INET, [{'addr':'No IP addr'}] )]
#     print ('%s: %s' % (ifaceName, ', '.join(addresses)))
# The first option is to run a thread in the background so you can continue
# doing things in your program.
client.loop_blocking()