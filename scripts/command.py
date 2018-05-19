import http.client
import urllib.parse
import json
import sys


def establish_connect(host, port):
    conn = None
    try:
        conn = http.client.HTTPConnection(host, port)
    except http.client.HTTPException:
        return conn, "Cannot connect to {}:{}".format(host, port)

    return conn, None


def send_request(conn, method, url):
    res = None
    try:
        conn.request(method, url)
        res = conn.getresponse()
    except http.client.HTTPException:
        return res, "Send {} request to {} failed".format(method, url)

    return res, None


def get_res(res):
    json_res = None
    json_res_str = None
    err = None
    try:
        json_res_str = res.read()
        json_res = json.loads(json_res_str.decode("utf-8"))
    except ValueError:
        err = "Cannot parse response: {}".format(json_res_str)

    return json_res, err


def check_res_error(res):
    return get_res(res)[1]


def send(host, port, transfer_id, dst_ip, src_dst_port, rate):
    conn, err = establish_connect(host, port)
    if err:
        return err

    parameters = {"dst": dst_ip, "port": src_dst_port, "rate": rate}
    url = "/send/{}?{}".format(transfer_id, urllib.parse.urlencode(parameters))

    res, err = send_request(conn, "POST", url)
    if err:
        return err

    return check_res_error(res)


def receive(host, port, transfer_id, src_dst_port):
    conn, err = establish_connect(host, port)
    if err:
        return err

    parameters = {"port": src_dst_port}
    url = "/receive/{}?{}".format(transfer_id, urllib.parse.urlencode(parameters))

    res, err = send_request(conn, "POST", url)
    if err:
        return err

    return check_res_error(res)


def get_progress(host, port, transfer_id):
    conn, err = establish_connect(host, port)
    if err:
        return err

    url = "/progress/{}".format(transfer_id)

    res, err = send_request(conn, "GET", url)
    if err:
        return err

    res_json, err = get_res(res)
    if err:
        return err

    return None, res_json


def update_rate(host, port, transfer_id, rate):
    conn, err = establish_connect(host, port)
    if err:
        return err

    parameters = {"rate": rate}
    url = "/updateRate/{}?{}".format(transfer_id, urllib.parse.urlencode(parameters))

    res, err = send_request(conn, "POST", url)
    if err:
        return err

    return check_res_error(res)


def terminate(host, port, transfer_id):
    conn, err = establish_connect(host, port)
    if err:
        return err

    url = "/terminate/{}".format(transfer_id)

    res, err = send_request(conn, "POST", url)
    if err:
        return err

    return check_res_error(res)


def active_transfer(host, port):
    conn, err = establish_connect(host, port)
    if err:
        return err

    url = "/activeTransfer"

    res, err = send_request(conn, "GET", url)
    if err:
        return err

    res_json, err = get_res(res)
    if err:
        return err

    return None, res_json


if __name__ == "__main__":
    server_host = "127.0.0.1"
    server_port = 9080

    if len(sys.argv) <= 1:
        print("Usage:")
        print("send [transfer id] [destination ip] [source and destination port] [rate]")
        print("receive [transfer_id] [source and destination port]")
        print("progress [transfer_id]")
        print("update [transfer_id] [rate]")
        print("terminate [transfer_id]")
        print("transfer")
        exit(0)
    error = "Invalid Command"
    if sys.argv[1] == "send":
        error = send(server_host, server_port, int(sys.argv[2]), sys.argv[3], int(sys.argv[4]), int(sys.argv[5]))

    if sys.argv[1] == "receive":
        error = receive(server_host, server_port, int(sys.argv[2]), int(sys.argv[3]))

    if sys.argv[1] == "progress":
        error, res_json = get_progress(server_host, server_port, int(sys.argv[2]))
        print("Type: {}".format(res_json["type"]))
        print("Progress: {}".format(res_json["progress"]))

    if sys.argv[1] == "update":
        error = update_rate(server_host, server_port, int(sys.argv[2]), int(sys.argv[3]))

    if sys.argv[1] == "terminate":
        error = terminate(server_host, server_port, int(sys.argv[2]))

    if sys.argv[1] == "transfer":
        error, res_json = active_transfer(server_host, server_port)
        print("Senders: {}".format(res_json["senders"]))
        print("Receivers: {}".format(res_json["receivers"]))

    if error:
        print(error)
