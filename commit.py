# -*- coding: utf-8 -*-

from influxdb import InfluxDBClient
from influxdb import SeriesHelper

# InfluxDB connections settings
host = '52.158.236.197'
port = 8086
dbname = 'ifashion'

myclient = InfluxDBClient(host=host, port=port, database=dbname)


class rfidseries(SeriesHelper):

    class Meta:
        client = myclient
        series_name = 'ifashion'
        # Defines all the fields (not indexed) in this time series.
        fields = ['category', 'color', "size", "offrack"]
        # Defines all the tags (indexed) for the series.
        tags = ['rackid', 'itemid']

        # Defines the number of data points to store prior to writing
        # on the wire.
        bulk_size = 5

        # autocommit must be set to True when using bulk_size
        autocommit = True


# The following will create *five* (immutable) data points.
# Since bulk_size is set to 5, upon the fifth construction call, *all* data
# points will be written on the wire via MySeriesHelper.Meta.client.
# rfidseries(server_name='us.east-1', some_stat=159, other_stat=10, time='')
# rfidseries(rackid='0', itemid="0")
# rfidseries(rackid='1', itemid="1")
# rfidseries(rackid='2', itemid="2")
# rfidseries(rackid='3', itemid="3")
# rfidseries(rackid='0', itemid="2")
# To inspect the JSON which will be written, call _json_body_():
#print(rfidseries._json_body_())
   
