import React, { useState, useEffect } from 'react';
import axios from 'axios';

const BACKEND_URL = 'https://5b1c-223-191-50-196.ngrok-free.app';

const IoT = () => {
  const [moistureLevel, setMoistureLevel] = useState('Loading...');
  const [pumpStatus, setPumpStatus] = useState('-');
  const [autoModeStatus, setAutoModeStatus] = useState('Automatic mode enabled');
  const [pumpCurrentStatus, setPumpCurrentStatus] = useState('off');
  const [loading, setLoading] = useState(true);

  const fetchMoistureLevel = async () => {
    try {
      const response = await axios.post(`${BACKEND_URL}/moisture`);
      setMoistureLevel(response.data.moisture_level);
      setLoading(false);
    } catch (error) {
      console.error('Error fetching moisture level:', error);
    }
  };

  const togglePump = async () => {
    try {
      if (pumpStatus === 'Sent Trigger to turn off pump') {
        await axios.post(`${BACKEND_URL}/pump/on`);
        setPumpStatus('Sent Trigger to turn on pump');
        setPumpCurrentStatus('on');
      } else {
        await axios.post(`${BACKEND_URL}/pump/off`);
        setPumpStatus('Sent Trigger to turn off pump');
        setPumpCurrentStatus('off');
      }
      setAutoModeStatus('Automatic mode disabled');
    } catch (error) {
      console.error('Error toggling pump:', error);
    }
  };

  const toggleAutoMode = async () => {
    try {
      await axios.post(`${BACKEND_URL}/pump/auto`);
      setAutoModeStatus(prevStatus =>
        prevStatus === 'Automatic mode disabled' ? 'Automatic mode enabled' : 'Automatic mode disabled'
      );
    } catch (error) {
      console.error('Error toggling automatic mode:', error);
    }
  };

  useEffect(() => {
    const interval = setInterval(() => {
      fetchMoistureLevel();
    }, 1000);

    return () => clearInterval(interval);
  }, []);

  useEffect(() => {
    if (autoModeStatus === 'Automatic mode enabled') {
      if (parseInt(moistureLevel) < 40) {
        setPumpCurrentStatus('on');
      } else {
        setPumpCurrentStatus('off');
      }
    }
  }, [autoModeStatus, moistureLevel]);

  return (
    <div className="bg-gray-100 p-4">
      {loading && (
        <div className="fixed inset-0 flex items-center justify-center bg-white">
          <div className="animate-spin rounded-full h-20 w-20 border-t-2 border-b-2 border-blue-500"></div>
        </div>
      )}

      <div className="max-w-xl mx-auto">

        <div className="mb-4">
          <h1 className="text-xl font-bold mb-2">Moisture Level:</h1>
          <p className="text-lg font-semibold" id="moisture-level">{moistureLevel}</p>
        </div>

        <div className="mb-4">
          <h2 className="text-xl font-bold mb-2">Pump Control:</h2>
          <button 
            className="px-4 py-2 bg-blue-500 text-white rounded hover:bg-blue-600"
            onClick={togglePump}
          >
            {pumpStatus === 'Sent Trigger to turn off pump' ? 'Turn On Pump' : 'Turn Off Pump'}
          </button>
          <div className="text-lg font-semibold text-center mb-2" id="pump-status">{pumpStatus}</div>
        </div>

        <div className="mb-4">
          <h2 className="text-xl font-bold mb-2">Automatic Mode Control:</h2>
          <button 
            className={`px-4 py-2 bg-green-500 text-white rounded hover:bg-green-600 ${autoModeStatus === 'Automatic mode disabled' ? 'text-red-500' : ''}`}
            onClick={toggleAutoMode}
          >
            Enable Automatic Mode
          </button>
          <div className={`text-lg font-semibold text-center mb-2 ${autoModeStatus === 'Automatic mode disabled' ? 'text-red-500' : ''}`} id="auto-status">{autoModeStatus}</div>
        </div>

        <div className="mb-4">
          <h2 className="text-xl font-bold mb-2">Pump Current Status:</h2>
          <p className="text-lg font-semibold text-center mb-2" id="pump-current-status">{pumpCurrentStatus}</p>
        </div>

      </div>
    </div>
  );
};

export default IoT;