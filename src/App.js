import React, {Component} from 'react';
import './App.css';
import Calendar from './Calendar.js';

class App extends Component {
  render() {
    return (
      <div className="App">
        <header className="App-header">
          <p>
              My Calendar App
          </p>
          <Calendar/>
        </header>
      </div>
    );
  }
}



export default App;
