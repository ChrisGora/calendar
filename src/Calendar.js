import React, {Component} from 'react';
import Button from 'react-bootstrap/Button'
import InputGroup from "react-bootstrap/InputGroup";
import FormControl from "react-bootstrap/es/FormControl";
import './Calendar.css';

const wasm = import("./a.out.wasm");

class Calendar extends Component {
  constructor(props) {
    super(props);
    const localDate = new Date();
    this.state = {
      selectedDay: localDate.getDate(),
      selectedMonth: localDate.getMonth() + 1,
      selectedYear: localDate.getFullYear(),
    }
  }

  handleClick(d) {
    this.setState({
      selectedDay: d,
    });
  }

  renderMonth(m, y) {

    // const api = {
    //   getDayOfWeek: Module.cwrap('getDayOfWeek', 'number', []),
    // };

    console.log("FUCKING HERE", wasm.then(wasm => {
      return wasm.getDayOfWeek();
    }));


    // m = m - 1;
    let d = 1;
    const firstDay = dayOfWeek(d, m, y);
    let month = [];

    let done = false;
    for (let i = 0; i < 6; i++) {
      let week = [];
      for (let j = 0; j < 7; j++) {
        if ((i !== 0 || j >= firstDay) && !done) {
          week = week.concat({val: d, i: i, j: j});
          d++;
          if (d > getDaysInAMonthConsideringLeap(m - 1)) {
            done = true;
          }
        } else {
          week = week.concat({val: null, i: i, j: j});
        }
      }
      month.push(week);
    }
    return month.map((a, i) => {
      return (
        <div key={i} className='row'>
          <div className='row-outer'> {a.map((b) => {
            return (
              <DayCell value={b.val}
                       key={b.i.toString() + b.j.toString()}
                       onClick={() => this.handleClick(b.val)}/>);
          })}
          </div>
        </div>
      );
    });
  }

  handleMonthChange = event => {
    this.setState({selectedMonth: event.target.value})
  };

  handleYearChange = event => {
    this.setState({selectedYear: event.target.value})
  };

  render() {
    return (
      <div>
        <div>
          <p>{this.state.selectedDay} {this.state.selectedMonth} {this.state.selectedYear}</p>
          <p>{getWeekdayString(dayOfWeek(this.state.selectedDay, this.state.selectedMonth, this.state.selectedYear))}</p>
          <p>{getDaysBetweenString(daysBetween(this.state.selectedDay, this.state.selectedMonth, this.state.selectedYear))}</p>
          {this.renderMonth(this.state.selectedMonth, this.state.selectedYear)}
        </div>
        <div>
          <InputGroup className="mt-5">
            <InputGroup.Prepend>
              <InputGroup.Text>Year and Month</InputGroup.Text>
            </InputGroup.Prepend>
            <FormControl type="number" value={this.state.selectedMonth} onChange={this.handleMonthChange}/>
            <FormControl type="number" value={this.state.selectedYear} onChange={this.handleYearChange}/>
          </InputGroup>
        </div>
      </div>
    );
  }
}

function DayCell(props) {
  return (
    <Button className="square" disabled={props.value === null} onClick={props.onClick}>
      {props.value}
    </Button>
  );
}


//----------------------------------------------------------------------------------------------------------------------

function getDaysBetweenString(n) {
  if (n === 0) {
    return "Today";
  } else {
    return `Days Between: ${n}`;
  }
}

function getWeekdayString(i) {
  switch (i) {
    case 0:
      return "Monday";
    case 1:
      return "Tuesday";
    case 2:
      return "Wednesday";
    case 3:
      return "Thursday";
    case 4:
      return "Friday";
    case 5:
      return "Saturday";
    case 6:
      return "Sunday";
    default:
      return "Invalid";
  }
}

function getDaysInAMonth(m) {
  const days = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
  return days[m];
}

function getDaysInAMonthConsideringLeap(m, y) {
  const days = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
  if (y !== null && m === 1 && !isLeap(y)) {
    return 28;
  }
  return days[m];
}

function isLeap(y) {
  const leapA = y % 4 === 0;
  const leapB = y % 100 !== 0;
  const leapC = y % 400 === 0;
  return leapA && (leapB || leapC);
}

function sumMonths(d, m, y) {
  let sum = 0;
  if ((!(isLeap(y)) && (m >= 3))) {
    sum = sum - 1;
  }
  m = m - 2;
  while (m >= 0) {
    sum = sum + getDaysInAMonth(m);
    m = m - 1;
  }
  return sum;
}

function sumYearsPre1900(y) {
  let sum = 0;
  while (y < 1899) {
    sum = sum + sumMonths(0, 13, (y + 1));
    y = y + 1;
  }
  return sum;
}

function sumYears(y) {
  if (y < 1900) {
    return sumYearsPre1900(y);
  }

  let sum = 0;
  while (y > 1900) {
    sum = sum + sumMonths(0, 13, (y - 1));
    y = y - 1;
  }
  return sum;
}

function dayRangeCheck(d, m) {
  return d <= getDaysInAMonth(m - 1);
}

function monthRangeCheck(m) {
  return !(m > 12 || m === 0);
}

function calendar(d, m, y) {
  let sum = d + sumMonths(d, m, y);
  if (y >= 1900) sum = sum + sumYears(y);
  else if (y < 1900) sum = sumYears(y) + sumMonths(0, 13, y) - sum;
  return sum;
}

function checkAll(d, m) {
  const checkA = dayRangeCheck(d, m);
  const checkB = monthRangeCheck(m);
  return checkA && checkB;
}

function daysBetween(d, m, y) {
  const localTime = new Date();
  const dToday = localTime.getDate();
  const mToday = localTime.getMonth() + 1;
  const yToday = localTime.getFullYear();
  const days = calendar(d, m, y);
  const daysToToday = calendar(dToday, mToday, yToday);

  let daysBetween;

  const A = (y > yToday);
  const B = (y === yToday) && (m > mToday);
  const C = (y === yToday) && (m === mToday) && (d >= dToday);
  const D = (y < 1900);
  if (A || B || C) {
    daysBetween = days - daysToToday;
  } else if (D) {
    daysBetween = days + daysToToday;
  } else {
    daysBetween = daysToToday - days;
  }
  return daysBetween;
}

function dayOfWeek(d, m, y) {
  d = d - 1;
  if (!checkAll(d, m)) {
    return -1;
  }
  let whichDay = calendar(d, m, y) % 7;
  if (y < 1900) whichDay = 7 - whichDay;
  return whichDay;
}

export default Calendar;