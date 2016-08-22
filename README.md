# Tera_Emulator_v1xxx
This is a WIP[Work in progress].

To compile it, you need:
      -boost code
      -mysql code

      v1010
      -ugly -buggy
      v1020
      -rsolved some bugs
      -more stabile
      -added future need code
      -max point[login->hit lobby]
      v1025
      -changed Client Recv and Send overall logic.
            -now send and receive are on separated threads
            -Send(byte*,unsigned int) mutexted function, adds ISendPackets to the queue
            -InternalSend(byte*,unsigned int) accessed only by Send thread, encrypts the data and sends it over
                        the network to client
            -receive gets 1 packet, processes it and adds packets to be sent to the queue
            -while receive thread -> othre threads may post packets [like chat system...] [NOT TESTED]
            -send thread pops all the elements in the queue [ISendPacket] and InternalSend-s them.
      data
       -added new data folder for the next release
            -abnormalities.bin [+ format description]
            -items.bin [+ format description]
            -effect.bin [+ format description]
            -bonus.bin [+ format description]
            -skills.bin [no format description yet]
      V1030
      -Updated for NA 4605 OpCode version.
      -finished lobby
      -Basic world.
