# Main script. Although this is not the entry point this collates all the pieces.
# This initializes the key functionalites such as serial port,threads etc

import threading
import time
from datetime import datetime
import logging

from threading import Lock
import serial
from serial.tools import list_ports
import config

import json

from blueprints.api import getCurrentProgr
from blueprints.threads import *
from flask import current_app as app

from apscheduler.schedulers.background import BackgroundScheduler

timeStarted = datetime.now()
checkL = ""
checkP = ""
schesuler = ""
# from apscheduler.schedulers.background import BackgroundScheduler
def initSerial():
    LOG = logging.getLogger(config.Config.APPLOGNAME)
    port = config.Hardware.SERIALPORT
    baud = config.Hardware.SERIALBAUD

    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(p)

    serial_port = serial.Serial(port, baud, timeout=None)
    # serial.flushInput()
    LOG.info(
        "Serial interface configured on "
        + port
        + "@"
        + str(baud)
        + " Pyserisal version: "
        + serial.VERSION
    )

    print(
        "Serial interface configured on "
        + port
        + "@"
        + str(baud)
        + " Pyserisal version: "
        + serial.VERSION
    )
    time.sleep(2)


def initialize():
    global scheduler
    LOG = logging.getLogger(config.Config.APPLOGNAME)

    # currentProgram = getCurrentProgr()
    initSerial()
    currentProgram = getCurrentProgr()
    # Setup and start the thread to read serial port
    thread_lock = Lock()
    thread = threading.Thread(
        target=readSer,
        args=(
            serial.Serial(
                config.Hardware.SERIALPORT, config.Hardware.SERIALBAUD, timeout=None
            ),
        ),
    )
    thread.start()

    # checkL = RepeatedTimer(
    #     int(config.Hardware.CHECKLIGHTSINTERVAL), checkLights, currentProgram
    # )
    scheduler = BackgroundScheduler()
    # scheduler.init_app(app)
    # replace_existing=True
    # scheduler.add_job(
    #     checkLights,
    #     "interval",
    #     args=currentProgram,
    #     seconds=2,
    #     id="12345",
    #     replace_existing=True,
    # )
    scheduler.start()

    # scheduler.add_job(
    #     readSer,
    #     "interval",
    #     seconds=int(10),
    #     id="checkS",
    #     args=[
    #         serial.Serial(
    #             config.Hardware.SERIALPORT, config.Hardware.SERIALBAUD, timeout=None
    #         )
    #     ],
    #     replace_existing=True,
    # )
    scheduler.add_job(
        checkLights,
        "interval",
        seconds=int(config.Hardware.CHECKLIGHTSINTERVAL),
        id="checkL",
        args=[currentProgram],
        replace_existing=True,
    )
    scheduler.add_job(
        activatePump,
        "interval",
        seconds=int(currentProgram["pumpStartEvery"]),
        id="checkP",
        args=[currentProgram],
        replace_existing=True,
    )

    # checkH = RepeatedTimer(24 * 60 * 60, guessHarvest, app)

    # checkP = RepeatedTimer(
    #     int(currentProgram["pumpStartEvery"]), activatePump, currentProgram
    # )

    timeNow = str(timeStarted.hour).zfill(2) + ":" + str(timeStarted.minute).zfill(2)
    LOG.info("System started. System time is: " + timeNow)
    LOG.info("Current program: " + str(currentProgram))
    LOG.info("Background jobs: " + str(scheduler.get_jobs()))
    # print(
    #     "System started. System time is: "
    #     + timeNow
    #     + " current program "
    #     + str(currentProgram)
    # )
