'use strict';
// Read GPS
const admin = require('firebase-admin');
const serviceAccount = require('./elcare-map-service-firebase-adminsdk-nb48c-b3b92f6f9d.json');
admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://elcare-map-service-default-rtdb.firebaseio.com"
});
var db=admin.database();
var userRef=db.ref("Location");
var sensor=db.ref("Gyro");

const line = require('@line/bot-sdk');
const express = require('express');
// create LINE SDK config from env variables
const config = {
  channelAccessToken: "TQIYpxJ2nepVhQY/7mPP9/AantyQfJOzZv5B2bUQBFpQV2QVCBe415GUhe0BuXc3Jj4TeUAyHL5WEuR9VJAHLHUPsFgAvwYee9/3ykJ/LH35znq9gHNfTY1RDILE2JF1Pafl6yvlz//DQzMcrQAjpwdB04t89/1O/w1cDnyilFU=",
  channelSecret: "390601629d9ac0986861684275148617",
};


/////////////////////////// MCU ///////////////////////////
const MCU = require('firebase-admin');
const serviceAccountMCU = require('./elcare-mcu-firebase-adminsdk-s9xye-ffd3ee92d2.json');
var otherApp = MCU.initializeApp({
      credential: MCU.credential.cert(serviceAccountMCU),
      databaseURL: "https://elcare-mcu-default-rtdb.firebaseio.com/"
// }, "Secondary");
}, "other");
// });
var otherAuth = otherApp.auth();
var dbMCU = otherApp.database();
var userMCU=dbMCU.ref("MCU");

////////////////อ่านค่า และส่งข้อมูลแจ้งเตือน //////////////////////////////
let promiseMo =  new Promise((resolve, reject) => {
  sensor.on('value', (snapshot) => {
  let Gx = snapshot.val().xValue;
  let Gy = snapshot.val().yValue;
  let Gz = snapshot.val().zValue;
  let alert = snapshot.val().Alert;
  console.log('Alert: ' + alert);
  console.log('xValue: ' + Gx);
  console.log('yValue: ' + Gy);
  console.log('zValue: ' + Gz);

  setTimeout(() => {
      resolve(alert);
  }, 1000); 
  // return resolve

  }, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
  });
});

let promiseMCU =  new Promise((resolve, reject) => {
  sensor.on('value', (snapshot) => {
  let Gx = snapshot.val().xValue;
  let Gy = snapshot.val().yValue;
  let Gz = snapshot.val().zValue;
  let alert = snapshot.val().Alert;
  console.log('Alert: ' + alert);
  console.log('xValue: ' + Gx);
  console.log('yValue: ' + Gy);
  console.log('zValue: ' + Gz);

  setTimeout(() => {
      resolve(alert);
  }, 1000); 
  // return resolve

  }, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
  });
});

///////////////////////////////////////////////////////////
promiseMo.then(value => {
  promiseMCU.then(value2 => {
    console.log(`Resolved Mobile: ${value}`);
    console.log(`Resolved MCU: ${value2}`);
    if(value == 1 || value2 == 1){
      userRef.on('value', (snapshot) => {
        //   console.log(snapshot.val().latitude);
            let lat = snapshot.val().latitude;
            let lon = snapshot.val().longitude;
            console.log(lat.toFixed(6)+" "+lon.toFixed(6))
            let url = "https://www.google.com/maps/place/";
            let location = url.concat(lat,",",lon);
            console.log(location)
            return client.pushMessage('U574e92cf98715d9fbe64031a35804e7b', { 
              type: 'text', 
              text: "กรุณาติดติดต่อกลับผู้ใช้งาน\n" + location + "คลิ๊กเพื่อ"
            });
        }, (errorObject) => {
          console.log('The read failed: ' + errorObject.name);
        });
    }
  });
});
console.log('Main program');
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// create LINE SDK client
const client = new line.Client(config);
const app = express();

// register a webhook handler with middleware
// about the middleware, please refer to doc
app.post('/callback', line.middleware(config), (req, res) => {
  Promise
    .all(req.body.events.map(handleEvent))
    .then((result) => res.json(result))
    .catch((err) => {
      console.error(err);
      res.status(500).end();
    });
});

// event handler
function handleEvent(event) {
  if (event.type !== 'message' || event.message.type !== 'text') {
    // ignore non-text-message event
    return Promise.resolve(null);
  }
  // เงื่อนไขการทำงาน
  else if(event.message.type == 'text' && event.message.text == 'Hello') {
    const payload = {
      type: "text",
      text: "Hello From Heroku Server."
    };
    return client.replyMessage(event.replyToken, payload);
  }

  else if(event.message.type == 'text' && event.message.text == 'ติดตามตัวผู้ใช้งาน' || event.message.type == 'text' && event.message.text == '2' ) {
      userRef.on('value', (snapshot) => {
        //   console.log(snapshot.val().latitude);
            let lat = snapshot.val().latitude;
            let lon = snapshot.val().longitude;
            console.log(lat.toFixed(6)+" "+lon.toFixed(6))
            let url = "https://www.google.com/maps/place/";
            let location = url.concat(lat,",",lon);
            console.log(location)

            const payload = {
              type: "text",
              text: "กดเพื่อแสดงตำแหน่งขอผู้ใช้งาน "+location
            };
            return client.replyMessage(event.replyToken, payload);

        }, (errorObject) => {
          console.log('The read failed: ' + errorObject.name);
        }); 
  }
  /////////////////////////
  else if(event.message.type == 'text' && event.message.text == 'สถานะรายวัน') {
    const payload = {
      type: "text",
      text: "ระบบอยู่ระหว่างการพัฒนา"
    };
    return client.replyMessage(event.replyToken, payload);
  }
  ///////////////////////////
  else if(event.message.type == 'text' && event.message.text == 'NetteN') {
    const payload = {
      type: "text",
      text: "DarkNEss"      
    };
    return client.replyMessage(event.replyToken, payload);
  }
  else if(event.message.type == 'text' && event.message.text == 'Net') {
    const payload = {
      type: "text",
      text: "NetteN"      
    };
    return client.replyMessage(event.replyToken, payload);
  }
  else if(event.message.type == 'text' && event.message.text == 'Log' || event.message.type == 'text' && event.message.text == 'log') {
    const payload = {
      type: "text",
      text: "ระบบกำลังอยู่ในการพัฒนา"      
    };
    return client.replyMessage(event.replyToken, payload);
  } 
  else if(event.message.type == 'text' && event.message.text == 'ติดต่อเจ้าหน้าที่' || event.message.type == 'text' && event.message.text == '3') {
    const payload = {
      type: "text",
      text: "กรุณากดลิงค์ "+"https://lin.ee/JpJoGuz"+" เพื่อทำการติดต่อเจ้าหน้าที่" 
    };
    return client.replyMessage(event.replyToken, payload);
  } 
  else if(event.message.type == 'text' && event.message.text == 'วิธีใช้งาน' || event.message.type == 'text' && event.message.text == '1') {
    const payload = {
      type: "text",
      text: "กรุณากดลิงค์\n"+"https://docs.google.com/document/d/1w544mMuhu_FLz6UzQ_2O9plA9f-6TpL9gShmj05AveQ/edit?usp=sharing"+"\nเพื่อทำการดูวิธีใช้งาน" 
    };
    return client.replyMessage(event.replyToken, payload);
  } 

  else if(event.message.type == 'text' && event.message.text == '11') {
    sensor.on('value', (snapshot) => {
          let Gx = snapshot.val().xValue;
          let Gy = snapshot.val().yValue;
          let Gz = snapshot.val().zValue;
          let alert = snapshot.val().Alert;

          const payload = {
            type: "text",
            text: "Alert: "+alert+"\nX: "+Gx+"\nY: "+Gy+"\nZ: "+Gz
          };
          return client.replyMessage(event.replyToken, payload);

      }, (errorObject) => {
        console.log('The read failed: ' + errorObject.name);
      }); 
  }
  else if(event.message.type == 'text' && event.message.text == '22') {
    userMCU.on('value', (snapshot) => {
      let alertHelp = snapshot.val().Alert;
      let valx = snapshot.val().valx;
      let valy = snapshot.val().valy;
      let valz = snapshot.val().valz;

      const payload = {
        type: "text",
        text: "Alert: "+alertHelp+"\nX: "+valx+"\nY: "+valy+"\nZ: "+valz
      };
      return client.replyMessage(event.replyToken, payload);
    }, (errorObject) => {
      console.log('The read failed: ' + errorObject.name);
    }); 
  }
  // else if(event.message.type == 'text' && event.message.text == '33') {
  //     // if(alertMo == 1 || alertHelp ==1){
  //       // userId = 'U574e92cf98715d9fbe64031a35804e7b'
  //       return client.pushMessage('U574e92cf98715d9fbe64031a35804e7b', { 
  //         type: 'text', 
  //         text: "hello, world : " + alert1 
  //       });
  //       // return client.pushMessage(event.replyToken, { 
  //       //   type: 'text', 
  //       //   text: 'NetteNXA' 
  //       // });
        
  //     // }
  // }



  
  else{
    const payload = {
      type: "text",
      text: "กรุณาพิมพ์ข้อความสำหรับการทำงานให้ถูกต้อง\n1. วิธีใช้งาน \n2. ติดตามตัวผู้ใช้งาน\n3. ติดต่อเจ้าหน้าที่"
    };
    return client.replyMessage(event.replyToken, payload);
  }
}



// listen on port
const port = process.env.PORT || 3000;
app.listen(port, () => {
  console.log(`listening on ${port}`);
});
