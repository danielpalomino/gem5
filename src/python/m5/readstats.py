def setReadstatsHandler(handler):
    '''Register a handler for the m5_readstats(length, offset) pseudo-instruction.

    The handler must take two arguments (length and offset) and return a string
    of at most length bytes starting from offset. The interface is analogous to
    the m5_readfile pseudo-instruction. It allows the simulated system to read
    a (pseudo-)file containing machine statistics information, e.g., McPat
    power output.
    '''
    global readstats
    readstats = handler
