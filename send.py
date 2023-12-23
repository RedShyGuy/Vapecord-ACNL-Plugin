import ftplib
import logging
import os
import socket

def ftp_upload(hostname, port, upload_src_path, upload_dst_path, timeout):
    logger.info({
        'action': 'ftp_upload',
        'status': 'run'
    })
    # FTP接続/アップロード
    with ftplib.FTP() as ftp:
        try:    
            ftp.connect(host=hostname, port=port, timeout=timeout)
            # パッシブモード設定
            ftp.set_pasv("true")

            with open(upload_src_path, 'rb') as fp:
                ftp.storbinary(upload_dst_path, fp)

        except ftplib.all_errors as e:
            logger.error({
                'action': 'ftp_upload',
                'message': 'FTP error = %s' % e
            })
    logger.info({
        'action': 'ftp_upload',
        'status': 'success'
    })


# logの設定
logger = logging.getLogger(__name__)
formatter = '%(asctime)s:%(name)s:%(levelname)s:%(message)s'
logging.basicConfig(
    filename='./ftp_logger.log',
    level=logging.DEBUG,
    format=formatter
)
logger.setLevel(logging.INFO)

hostname = ""
sendpost_path = "sendpost.txt"

if not os.path.exists(sendpost_path):
    print(f"`{sendpost_path}` is not found.")
    exit(1)

with open(sendpost_path) as f:
    hostname = f.readline().replace("\n", "")

print("ファイルの種類\nplugin: 1\n-> ")
file_select = input("")
if int(file_select) == 1:
    upload_src_path = "./Vapecord-ACNL-Plugin.3gx" 
    upload_dst_path = "STOR /luma/plugins/0004000000082600/Vapecord-ACNL-Plugin.3gx" 
else:
    print("例外: 存在しない値")
    exit(1)

port = 5000
timeout = 500

try:
    logger.info("===START FTP===")
    ftp_upload(hostname, port, upload_src_path, upload_dst_path, timeout)
    logger.info("===FINISH FTP===")
except socket.error as e:
    print(e)

print("ok!!")
