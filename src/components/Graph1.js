import { Chart, registerables } from 'chart.js';
import { Line } from 'react-chartjs-2';
Chart.register(...registerables);


function Graph(props) {
  const data = {
    labels: props.timeData,


    datasets: [
      {
        data: props.data,
        label: 'Temprature',
        fill: false,
        backgroundColor: "#2e4355",
        pointBorderColor: "#8884d8",
        pointBorderWidth: 3,
        borderColor: 'white',

        // pointRadius:8,
        // tension: 0.4
      },
    ],
  };


  const options = {

    plugins: {

      legend: {
        display: false
      },
    },

    layout: {
      padding: {
        bottom: 100,
        top: 20

      }

    },
    scales: {
      y: {
        display: true,
        title: {
          display: true,
          text: "Sensor1",
          font: {
            size: 25

          }
        },
        ticks: {
          color: "white",
          font: {
            size: 18

          }
        },
        grid: {
          color: "#243240"
        }
      },
      x: {
        display: true,
        title: {
          display: true,
          text: "Time",
          font: {
            size: 25

          }
        },
        ticks: {
          color: "white",
          font: {
            size: 18
          }
        }
      }
    },
  };

  return <Line data={data} options={options} />


}

export default Graph