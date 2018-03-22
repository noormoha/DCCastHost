FORMAT: 1A

# DCCast Host

## Send [/send]

### Start a Sending [POST /send/{transferId}{?dst,port,volume,rate}]

A send request will start a NORM sender and send data from its own IP address and port number **port** to IP address **dst** and port number **port**.

+ Parameters

    + transferId: `0` (number) - An unique integer identifier of transfer seesion with a range [0, 5000].
    + dst: `127.0.0.1` (string) - The desination IP address as a string.
    + port: `1000` (number) - An integer with a range [1000, 9999]. The port number that will be used as both source port and destination port.
    + volume: `1024` (number) - An integer with a range [1, 2^64] that indicate how many bytes the sender should send to destination.
    + rate: `500` (number) - An integer with a range[1, 2^32] that indicate how many bytes the sender should send in 1 second.

+ Response 200 (application/json)
    
    + Attribute (object)
        + error (string, nullable) - Must be null.
        
    + Body
    
            {"error": null}

+ Response 400 (application/json)

    + Attribute (object)
        + error (string) - Reason of the error.

    + Body
    
            {"error": "Reason of the error"}
        
## Receive [/receive]

### Start a Receiving [POST /receive/{transferId}{?src,port}]

A receive request will start a NORM receiver and wait for data from the IP address **src** and port number **port**.

+ Parameters

    + transferId: `0` (number) - An unique integer identifier of transfer seesion with a range [0, 5000].
    + src: `127.0.0.1` (string) - The IP address of sender.
    + port: `1000` (number) - An integer with a range [1000, 9999]. The port number that will be used as both source port and destination port.

+ Response 200 (application/json)
    
    + Attribute (object)
        + error (string, nullable) - Must be null.
        
    + Body
    
            {"error": null}

+ Response 400 (application/json)

    + Attribute (object)
        + error (string) - Reason of the error.

    + Body
    
            {"error": "Reason of the error"}
        
## Update Rate [/updaterate]

### Update the Rate of a Sender [POST /updaterate/{transferId}{?rate}]

An update rate request will update rate limit of the sender with id **transferId**.

+ Parameters 

    + transferId: `0` (number) - An unique integer identifier of transfer seesion with a range [0, 5000].
    + rate: `500` (number) - An integer with a range[1, 2^32] that indicate how many bytes the sender should send in 1 second.

+ Response 200 (application/json)
    
    + Attribute (object)
        + error (string, nullable) - Must be null.
        
    + Body
    
            {"error": null}

+ Response 400 (application/json)

    + Attribute (object)
        + error (string) - Reason of the error.

    + Body
    
            {"error": "Reason of the error"}
        
## Progress [/progress]

### Check Progress [GET /progress/{transferId}]

Return how many bytes a sender/receiver has proccessed.

+ Parameters

    + transferId: `0` (number) - An unique integer identifier of transfer seesion with a range [0, 5000].

+ Response 200 (application/json)

    + Attributes (object)
        + error (string, nullable) - Must be null.
        + type (string) - Either a "receiver" or a "sender". 
        + progress (number) - Bytes that have been sent/received.
    
    + Body
    
            {"error": null, "type": "sender", "progress": 3424}
            {"error": null, "type": "receiver", "progress": 5372834}

+ Response 400 (application/json)

    + Attributes (object)
        + error (string, nullable) - Reason of the error.
    
    + Body
    
            {"error": "Reason of the error"}
        
## Terminate [/terminate]

### Forece a Termination [POST /terminate/{transferId}]

Force a sender/receiver to stop.

+ Parameters

     + transferId: `0` (number) - An unique integer identifier of transfer seesion with a range [0, 5000].

+ Response 200 (application/json)
    
    + Attribute (object)
        + error (string, nullable) - Must be null.
        
    + Body
    
            {"error": null}

+ Response 400 (application/json)

    + Attribute (object)
        + error (string) - Reason of the error.

    + Body
    
            {"error": "Reason of the error"}
            
## Clean [/cleanall]

### Clean Any Transmission [POST /cleanall]

Force all senders/receivers to stop.

+ Response 200 (application/json)
    
    + Attribute (object)
        + error (string, nullable) - Must be null.
        
    + Body
    
            {"error": null}

+ Response 400 (application/json)

    + Attribute (object)
        + error (string) - Reason of the error.

    + Body
    
            {"error": "Reason of the error"}


        