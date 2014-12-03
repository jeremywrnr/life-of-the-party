Lfe of the Party
=================

LED Configuration Instructions

To connect to the light (MAC):
    click ok, exit configuration page
 
1. select the moodL from the wireless networks
2. open 'networks preference pane'
3. click the 'advanced button', lower right
4. a new configuration page should pop up
5. set IPv4 addr: 192.168.0.2
6. set subnet mask: 255.255.255.0
7. set router: 192.168.0.2
8. configure IPv6: link-local only
9. click ok, exit configuration page
10. click apply on the network page

Test the connection:

    ping 192.168.0.3

Test lights (rgb):

    python varcolor 1 1 1
Default changed to Arduino 1.0 + compatibility by 
Daniel Taub on 2013-01-29


Forked by Brian Neltner on 2010-10-06
