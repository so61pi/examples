# -*- coding: utf-8 -*-
from __future__ import unicode_literals

import json, random, time
import threading
from multiprocessing import Queue
from Queue import Empty
from datetime import datetime

from django.shortcuts import render

# Create your views here.
from django.http import HttpResponse


gQueue = Queue(maxsize=14*128*10*10)


def ranarr(n):
    return random.sample(range(1, 100), n)


def create_random_signal(id):
    min = 0;
    max = 100;
    millis = int(round(time.time() * 1000))
    return {'id': id,
            'data': {'time' : millis,
                     'state' : "abx",
                     'AF3' : random.uniform(min, max),
                     'F7' : random.uniform(min, max),
                     'F3' : random.uniform(min, max),
                     'FC5' : random.uniform(min, max),
                     'T7' : random.uniform(min, max),
                     'P7' : random.uniform(min, max),
                     'O1' : random.uniform(min, max),
                     'O2' : random.uniform(min, max),
                     'P8' : random.uniform(min, max),
                     'T8' : random.uniform(min, max),
                     'FC6' : random.uniform(min, max),
                     'F4' : random.uniform(min, max),
                     'F8' : random.uniform(min, max),
                     'AF4' : random.uniform(min, max)
                    }
           }


thread_created = False
def create_random_data_thread():
    while True:
        gQueue.put(create_random_signal('x'))
        gQueue.put(create_random_signal('y'))
        time.sleep(0.125)


def get_random_data():
    data = []
    try:
        while gQueue.empty() == False:
            data.append(gQueue.get_nowait())
    except Empty:
        pass
    finally:
        return data


def index(request):
    return render(request, 'testapp/index.html')


def data(request):
    global thread_created
    if thread_created == False:
        t = threading.Thread(target=create_random_data_thread)
        t.start()
        thread_created = True
    data = get_random_data()
    return HttpResponse(json.dumps(data), content_type='application/json')
