import React, { useState, useEffect } from 'react';
import Graph from './Graph1'
import Graph2 from './Graph2';
const axios = require('axios');


function Home() {

  const [sensor_data1, setSensor_Data1] = useState([])
  const [sensor_data2, setSensor_Data2] = useState([])
  const [timeStamp, setTimeStamp] = useState([]);
  let [seconds, setSeconds] = useState([])
  const [buttonText2, setButtonText2] = useState("ON_2");
  const [buttonState2, setButtonState2] = useState(false)
  const [buttonText1, setButtonText1] = useState("ON_1");
  const [buttonState1, setButtonState1] = useState(false)
  const [buttonLabel, setbuttonLabel] = useState(false)

  let data1, data2, timeData;
  let dataURL = 'https://iot-task0.herokuapp.com/api/readings/withtime'
  let buttonData = 'https://iot-task0.herokuapp.com/api/interface/buttonstate'

  useEffect(() => {

    fetch(dataURL)
      .then((response) => response.json())
      .then((json) => {
        data1 = json.temperature
        data2 = json.distance
        timeData = json.time
        setSensor_Data1(data1)
        setSensor_Data2(data2)
        setTimeStamp(timeData)
      })

  }, [setInterval(() => {
    seconds = seconds + 1
  }, 5000)]);

  const postReq1 = () => {
    setButtonState1(!buttonState1);
    setButtonText1(buttonState1 ? "ON_1" : "OFF_1");

    let data = JSON.stringify({
      "buttonState": buttonState1
    });

    let config = {
      method: 'post',
      url: buttonData,
      headers: {
        'Content-Type': 'application/json'
      },
      data: data
    };
    axios(config)
      .then((response) => {
        console.log(JSON.stringify(response.data));
      })
      .catch((error) => {
        console.log(error);
      });
    console.log(data);
  };
  const postReq2 = () => {
    setButtonState2(!buttonState2);
    setButtonText2(buttonState2 ? "ON_2" : "OFF_2");
    setbuttonLabel(!buttonLabel)
  };

  // useEffect(() => {

  // // const BODY = 'hello'

  // fetch(buttonData, {
  //     method: 'POST',
  //     headers:{'Content-type': 'application/json'},
  //     body: JSON.stringify(buttonText1),

  // })
  // // console.log(JSON.stringify(BODY))
  // // console.log(buttonState)
  // // console.log("done")
  // },[buttonState1])

  // useEffect(() => {

  //     // const BODY = 'hello'

  //     fetch(buttonData, {
  //         method: 'POST',
  //         headers:{'Content-type': 'application/json'},
  //         body: JSON.stringify(buttonText2),

  //     })
  //     // console.log(JSON.stringify(BODY))
  //     // console.log(buttonState)
  //     // console.log("done")
  //     },[buttonState2])

  return (
    <div>

      <div >
        <select name="selectList" id="selectList" className={'toggle--button'}>
          <option value="option 1" className={'text'}>ICU 1</option>
          <option value="option 2">ICU 2</option>
        </select>
      </div>


      <h2>Patient1</h2>
      <div className='chart1'>
        <Graph data={sensor_data1} timeData={timeStamp} />
        <button onClick={postReq1} className={'button ' + (buttonState1 ? 'button--off' : '')}>{buttonText1}</button>
      </div>


      <h2>Patient2</h2>

      <div className='chart2'>
        <Graph2 data={sensor_data2} timeData={timeStamp} title={'Temprature'} />
        <button onClick={postReq2} className={'button ' + (buttonState2 ? 'button--off' : '')}>{buttonText2}</button>
      </div>


    </div>
  )



}

export default Home